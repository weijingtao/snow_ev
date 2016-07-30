//
// Created by weitao on 7/14/16.
//

#include "connection.h"

namespace snow
{
    connection::connection(tcp_socket& socket)
        : m_socket(socket) {
        assert(m_socket.get_socket_fd() > 0);
    }

    connection::~connection() {
        m_timer.cancel();
        m_io_event.disable_all();
    }

    void connection::start() {

    }

    void connection::stop() {

    }

    void connection::send(const buffer& data) {
        if(m_send_buffer.readable_bytes() > 0) {
            m_send_buffer.append(data.read_index(), data.readable_bytes());
            m_socket.write(m_send_buffer);
        } else {
            m_socket.write(const_cast<buffer&>(data));
            if(data.readable_bytes() > 0) {
                m_send_buffer.append(data.read_index(), data.readable_bytes());
            }
        }
    }

    void connection::handle_read() {
        std::size_t ret = m_socket.read(&m_recv_buffer);
        if((0 == ret) || ((ret < 0) && (errno != EAGAIN && errno != EWOULDBLOCK))) {
            m_timer.cancel();
            m_io_event.disable_all();
            return;
        }
        std::size_t pkg_len = 0;
        while ((pkg_len = m_pkg_spliter(m_recv_buffer.read_index(), m_recv_buffer.readable_bytes())) > 0) {
            m_dispatcher(buffer(m_recv_buffer.read_index(), pkg_len));
            m_recv_buffer.increase_read_index(pkg_len);
        }
    }

    void connection::handle_write() {
        ssize_t write_bytes = m_socket.write(m_send_buffer);
        if(m_send_buffer.readable_bytes() > 0) {
            m_io_event.enable_reading();
            m_io_event.enable_oneshot();
        }
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