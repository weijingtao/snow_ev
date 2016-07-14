//
// Created by weitao on 7/14/16.
//

#include "connection.h"

namespace snow
{
    connection::connection(int socket_fd)
        : m_socket_fd(socket_fd) {
        assert(m_socket_fd > 0);
        enable_nonblock(m_socket_fd);
    }

    connection::~connection() {
        m_timer->cancel();
        m_io_event->disable_all();
        ::close(m_socket_fd);
    }

    void connection::start() {

    }

    void connection::stop() {

    }

    void connection::send(const buffer& data) {
        if(m_send_buffer.readable_bytes() > 0) {
            iovec ios[2];
            ios[0].iov_base = m_send_buffer.read_index();
            ios[0].iov_len  = m_send_buffer.readable_bytes();
            ios[1].iov_base = const_cast<char*>(data.read_index());
            ios[1].iov_len  = data.readable_bytes();
            std::size_t write_bytes = writev(m_socket_fd, ios, sizeof(ios)/sizeof(iovec));
            if(write_bytes > 0) {
                if(write_bytes == (m_send_buffer.readable_bytes() + data.readable_bytes())) {
                    return;
                }
                if(write_bytes > m_send_buffer.readable_bytes()) {
                    std::size_t old_readable_bytes = m_send_buffer.readable_bytes();
                    m_send_buffer.increase_read_index(m_send_buffer.readable_bytes());
                    m_send_buffer.append(data.read_index() + (write_bytes - old_readable_bytes), (data.readable_bytes() + old_readable_bytes - write_bytes));
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
                m_send_buffer.append(data.read_index() + write_bytes, data.readable_bytes() - write_bytes);
                return;
            } else {
                m_send_buffer.append(data.read_index(), data.readable_bytes());
                return;
            }
        }
    }

    void connection::handle_read() {
        ssize_t read_bytes = 0;
        while (true){
            m_recv_buffer.ensure_writeable_bytes(1500);
            read_bytes = ::read(m_socket_fd, m_recv_buffer.write_index(), m_recv_buffer.writeable_bytes());
            if(0 == read_bytes) {
                m_timer->cancel();
                m_io_event->disable_all();
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
            m_recv_buffer.increase_read_index(pkg_len);
        }
    }

    void connection::handle_write() {
        ssize_t write_bytes = 0;
        do {
            while((m_send_buffer.readable_bytes() > 0) && ((write_bytes = write(m_socket_fd, m_send_buffer.read_index(), m_send_buffer.readable_bytes())) > 0)) {
                m_send_buffer.increase_read_index(write_bytes);
            }
            if (errno == EINTR) {
                continue;
            }
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                break;
            }
        } while(true);
    }

    void connection::handle_timeout() {
        m_io_event->disable_all();
        delete this;
    }
}