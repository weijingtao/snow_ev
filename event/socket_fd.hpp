#pragma once

class socket_fd
{
public:
    socket_fd(int fd = -1)
            : m_fd{fd} {
    }

    socket_fd& operator=(int fd) {
        m_fd = fd;
        return *this;
    }

    operator int() const {
        return m_fd;
    }

    operator bool() const {
        return m_fd >= 0;
    }

private:
    int m_fd;
};