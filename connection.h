//
// Created by weitao on 7/2/16.
//

#ifndef SNOW_LIBEV_CONNECTION_H
#define SNOW_LIBEV_CONNECTION_H

#ifndef _SNOW_CONNECTION_HPP
#define _SNOW_CONNECTION_HPP

#include <sys/uio.h>
#include <memory>
#include <functional>
#include "ev.h"
#include "buffer.h"
void co_enable_nonblock(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

class request;
class buffer;
class connection
{
public:
    typedef std::function<void(const buffer&)> dispatcher_type;
    typedef std::function<std::size_t(const char*const, std::size_t)> pkg_split_type;

    connection(int socket_fd, dispatcher_type& dispatcher)
        : m_socket_fd(socket_fd),
          m_dispatcher(std::move(dispatcher)) {
        assert(m_socket_fd > 0);
        enable_nonblock(fd);
    }

    ~connection() {
        ev_io_stop(loop, m_io_watcher.get());
        ev_timer_stop(loop, m_timer_watcher);
    }

    void start() {

    }

    void stop() {

    }

    void send(const buffer& data) {
        if(m_send_buffer.readable_bytes() > 0) {
            iovec ios[2];
            ios[0].iov_base = m_send_buffer.read_index();
            ios[0].iov_len  = m_send_buffer.readable_bytes();
            ios[1].iov_base = data.read_index();
            ios[1].iov_len  = data.readable_bytes();
            std::size_t write_bytes = writev(m_socket_fd, &ios, sizeof(ios)/sizeof(iovec));
            if(write_bytes > 0) {
                if(write_bytes == (m_send_buffer.readable_bytes() + data.readable_bytes())) {
                    return;
                }
                if(write_bytes > m_send_buffer.readable_bytes()) {
                    std::size_t old_readable_bytes = m_send_buffer.readable_bytes();
                    m_send_buffer.increase_read_index(m_send_buffer.readable_bytes());
                    m_send_buffer.apped(data.read_index() + (write_bytes - old_readable_bytes), (data.readable_bytes() + old_readable_bytes - write_bytes));
                    return;
                }
                if(write_bytes <= m_send_buffer.readable_bytes()) {
                    m_send_buffer.increase_read_index(write_bytes);
                    m_send_buffer.append(data.read_index(), data.readable_bytes());
                    return;
                }
            } else {
                m_send_buffer.append(data.read_index(), data.readable_bytes());
                return;
            }
        }
        else {
            std::size_t write_bytes = write(m_socket_fd, data.read_index(), data.readable_bytes());
            if(write_bytes > 0) {
                m_send_buffer.apped(data.read_index() + write_bytes, data.readable_bytes() - write_bytes);
                return;
            } else {
                m_send_buffer.append(data.read_index(), data.readable_bytes());
                return;
            }
        }
    }

private:
    static void handle_read(ev_loop* loop, ev_io_watcher* watcher, int revents) {
        ssize_t read_bytes = 0;
        while (true){
            m_recv_buffer.ensure_writeable_bytes(1500);
            read_bytes = read(fd, m_recv_buffer.write_index(), m_recv_buffer.writeable_bytes());
            if(0 == read_bytes) {
                ev_io_stop(loop, watcher);
                delete this;
            }
            if (read_bytes > 0) {
                m_recv_buffer.increase_write_index(read_bytes);
            }
            else {
                if (errno == EINTR) {
                    continue;
                }
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    break;
                }
            }
        }
        std::size_t pkg_len = 0;
        while ((pkg_len = m_pkg_spliter(m_recv_buffer.read_index(), m_recv_buffer.readable_bytes())) > 0) {
            m_dispatcher(buffer(m_recv_buffer.read_index(), pkg_len));
            m_recv_buffer.increase_read_index();
        }
    }

    static void handle_write(ev_loop* loop, ev_io_watcher* watcher, int revents) {
        size_t left_bytes   = len;
        ssize_t write_bytes = 0;
        const char *p = buffer;
        do {
            while((m_send_buffer.readable_bytes() > 0) && ((write_bytes = write(m_socket_fd, m_send_buffer.read_index(), m_send_buffer.readable_bytes)) > 0)) {
                m_send_buffer.increase_read_index();
            }
            if (errno == EINTR) {
                continue;
            }
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                break;
            }
        } while(true);
    }

    static void handle_timeout(ev_loop* loop, ev_timer_watcher* watcher, int revents) {
        auto* connection_ptr = static_cast<connection*>(watcher->data);
        ev_io_stop(loop, m_io_watcher.get());
        ev_timer_stop(loop, m_timer_watcher);
        delete connection_ptr;
    }

private:
    int m_socket_fd;
    dispatcher_type m_dispatcher;
    pkg_split_type  m_pkg_spliter;
    buffer          m_recv_buffer;
    buffer          m_send_buffer;
    std::unique_ptr<ev_io_watcher>    m_io_watcher;
    std::unique_ptr<ev_timer_watcher> m_timer_watcher;
};


#endif //_SNOW_CONNECTION_HPP

#endif //SNOW_LIBEV_CONNECTION_H
