//
// Created by weitao on 7/14/16.
//

#include "buffer.h"

namespace snow
{
    buffer::buffer(std::size_t init_size)
        : m_buffer(new char[init_size]),
          m_size(init_size),
          m_read_index(0),
          m_write_index(0) {
        SNOW_LOG_TRACE << "buffer construct\n";
    }

    buffer::buffer(const char* buffer, std::size_t len)
        : m_buffer(new char[len]),
          m_size(len),
          m_read_index(0),
          m_write_index(0) {
        std::copy(buffer, buffer + len, m_buffer.get());
    }

    //TODO
    buffer::buffer(snow::buffer&& rhs)
        : m_size(rhs.m_size),
          m_read_index(rhs.m_read_index),
          m_write_index(rhs.m_write_index) {
        m_buffer.swap(rhs.m_buffer);
        rhs.m_size        = 0;
        rhs.m_read_index  = 0;
        rhs.m_write_index = 0;
        SNOW_LOG_TRACE << "buffer move construct";
    }

    buffer::~buffer() {
        SNOW_LOG_TRACE << "buffer destruct";
    }

    void buffer::append(const char * data, std::size_t len) {
        ensure_writeable_bytes(len);
        std::copy(data, data + len, read_index());
        increase_write_index(len);
    }

    void buffer::ensure_writeable_bytes(std::size_t size) {
        if(writeable_bytes() >= size) {
            return;
        }
        if (m_size - readable_bytes() < size) {
            std::unique_ptr<char[]> new_buffer(new char[m_write_index + size]);
            std::copy(read_index(), write_index(), new_buffer.get());
            m_buffer.swap(new_buffer);
        }
        std::size_t readable = readable_bytes();
        m_read_index         = 0;
        m_write_index        = m_read_index + readable;
        assert(writeable_bytes() >= size);
    }
}