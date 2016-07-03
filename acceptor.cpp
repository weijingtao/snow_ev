//
// Created by weitao on 7/1/16.
//

#include "acceptor.h"

#include <ev.h>

acceptor::acceptor(ev_loop *loop)
  : acceptor(loop, "", 0) {

}

acceptor::acceptor(ev_loop *loop, const std::string &ip, uint16_t port)
  : m_loop(loop),
    m_io_watcher(new ev_io),
    m_port(0) {

}

acceptor::~acceptor() {
    if(nullptr != m_io_watcher) {
        delete m_io_watcher;
        m_io_watcher = nullptr;
    }
    m_loop = nullptr;
}