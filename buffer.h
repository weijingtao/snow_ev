//
// Created by weitao on 7/2/16.
//

#pragma once

#include <cassert>
#include <cstring>
#include <memory>
#include <iostream>
#include "logger/logger.h"

namespace snow
{
    class buffer
    {
    public:
        static const std::size_t INIT_SIZE = 1024;
        explicit buffer(std::size_t init_size = INIT_SIZE);

        buffer(const char* buffer, std::size_t len);

        //TODO
        buffer(buffer&& rhs);

        ~buffer();

        std::size_t readable_bytes() const {
            return m_write_index - m_read_index;
        }

        std::size_t writeable_bytes() const {
            return m_size - m_write_index;
        }

        char* read_index() {
            return m_buffer.get() + m_read_index;
        }

        const char* read_index() const {
            return m_buffer.get() + m_read_index;
        }

        char* write_index() {
            return m_buffer.get() + m_write_index;
        }

        const char* write_index() const {
            return m_buffer.get() + m_write_index;
        }

        void increase_read_index(std::size_t size) {
            m_read_index += size;
        }

        void increase_write_index(std::size_t size) {
            m_write_index += size;
        }

        void append(const char * data, std::size_t len);

        void ensure_writeable_bytes(std::size_t size);

    private:
        std::unique_ptr<char[]> m_buffer;
        std::size_t             m_size;
        std::size_t             m_read_index;
        std::size_t             m_write_index;
    };
}
