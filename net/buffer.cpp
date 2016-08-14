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
        m_write_index += len;
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
        SNOW_LOG_DEBUG << "data len " << len;
        ensure_writeable_bytes(len);
        std::copy(data, data + len, write_index());
        increase_write_index(len);
    }

    void buffer::ensure_writeable_bytes(std::size_t size) {
        SNOW_LOG_DEBUG << "size " << size;
        if(writeable_bytes() >= size) {
            SNOW_LOG_DEBUG << "courent size enough, return direct";
            return;
        }
        if (m_size - readable_bytes() < size) {
            SNOW_LOG_DEBUG << "create a now buffer size " << m_write_index + size;
            std::unique_ptr<char[]> new_buffer(new char[m_write_index + size]);
            SNOW_LOG_DEBUG << "read index " << read_index() << " write index " << write_index();
            std::copy(read_index(), write_index(), new_buffer.get());
            m_buffer.swap(new_buffer);
            m_size = m_write_index + size;
        }
        std::size_t readable = readable_bytes();
        m_read_index         = 0;
        m_write_index        = m_read_index + readable;
        SNOW_LOG_DEBUG << "total size " << m_size << " readable bytes " << m_write_index - m_read_index << " writeable bytes " << m_size - m_write_index;
        assert(writeable_bytes() >= size);
    }
}