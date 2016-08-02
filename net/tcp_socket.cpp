//
// Created by weitao on 7/16/16.
//

#include "tcp_socket.h"

#include <sys/uio.h>
#include <unistd.h>
#include <errno.h>
#include <cassert>

namespace snow
{
    tcp_socket::tcp_socket(int fd)
        : socket(fd) {

    }

    std::size_t tcp_socket::read(buffer* buf) {
        thread_local static char common_buffer[65536];
        ssize_t read_bytes = 0;
        while (true){
            buf->ensure_writeable_bytes(1500);
            struct iovec iov[2] = {0};
            iov[0].iov_base = buf->write_index();
            iov[0].iov_len  = buf->writeable_bytes();
            iov[1].iov_base = common_buffer;
            iov[1].iov_len  = sizeof(common_buffer);
            read_bytes = ::readv(m_fd, iov, sizeof(iov)/sizeof(iovec));
            if(0 == read_bytes) {
                return 0;
            }
            if (read_bytes > 0) {
                std::size_t writeable_bytes = buf->writeable_bytes();
                buf->increase_write_index(buf->writeable_bytes());
                if(read_bytes > buf->writeable_bytes()) {
                    buf->append(common_buffer, read_bytes - writeable_bytes);
                }
                return read_bytes;
            }
            else {
                if (errno == EINTR) {
                    continue;
                }
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    return -1;
                }
                return -1;
            }
        }
    }

    std::size_t tcp_socket::write(buffer& buf) {
        assert(buf.readable_bytes() > 0);
        do {
            std::size_t write_bytes = ::write(m_fd, buf.read_index(), buf.readable_bytes());
            if(write_bytes > 0) {
                buf.increase_read_index(write_bytes);
                return write_bytes;
            } else {
                if (errno == EINTR) {
                    continue;
                }
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    return -1;
                }
                return -1;
            }
        } while(true);
    }

    bool tcp_socket::listen() {
        if(0 == ::listen(m_fd, 10)) {
            return true;
        } else {
            SNOW_LOG_FATAL << "listen failed fd:" << m_fd << " errno:" << errno << " errmsg:" << strerror(errno);
            return false;
        }
    }

    void tcp_socket::accept(std::vector<tcp_socket> *sockets) {
        assert(nullptr != sockets);
        do {
            sockaddr addr = {0};
            socklen_t addr_len = 0;
            int client_fd = ::accept(m_fd, &addr, &addr_len);
            if(client_fd >= 0) {
                sockets->emplace_back(client_fd);
            } else {
                if (errno == EINTR) {
                    continue;
                }
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    break;
                }
            }
        } while(true);
    }
}