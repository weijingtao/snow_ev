//
// Created by weitao on 7/14/16.
//

#include "connection.h"

namespace snow
{
    connection::connection(tcp_socket&& socket)
        : m_socket(std::move(socket)) {
        assert(m_socket.get_socket_fd() > 0);
    }

    connection::~connection() {
        m_timer.cancel();
        m_io_event.disable_all();
        handle_close();
    }

    void connection::start() {
        m_io_event.enable_reading();
    }

    void connection::stop() {
        m_io_event.disable_all();
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
            handle_close();
            return;
        }
        std::size_t pkg_len = 0;
        while ((pkg_len = m_pkg_spliter(m_recv_buffer.read_index(), m_recv_buffer.readable_bytes())) > 0) {
            m_dispatcher(m_recv_buffer.read_index(), pkg_len, std::bind(&connection::send, this, std::placeholders::_1));
            m_recv_buffer.increase_read_index(pkg_len);
        }
    }

    void connection::handle_write() {
        ssize_t write_bytes = m_socket.write(m_send_buffer);
        if(m_send_buffer.readable_bytes() > 0) {
            m_io_event.enable_reading();
            m_io_event.enable_oneshot();
        }
    }

    void connection::handle_timeout() {
        m_io_event.disable_all();
        handle_close();
    }

    void connection::handle_close() {
        if(m_close_handler) {
            m_close_handler(m_index);
        }
    }
}