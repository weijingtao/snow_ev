//
// Created by weitao on 7/4/16.
//

#include "server.h"

server::server()
    : m_stop_flag(true),
      m_thread_poll(2) {

}

server::~server() {
    stop();
}

void server::start() {
    m_stop_flag = false;
    m_thread_poll.start(std::bind(&server_base::run, this));
}

void server::stop() {
    m_stop_flag = true;
}

int server::init() {
    return 0;
}

void server::run() {
    SNOW_LOG_TRACE << "server runing" << std::endl;
    m_proxy.set_pkg_spliter(std::bind(&server_base::pkg_check, this, std::placeholders::_1, std::placeholders::_2));
    m_proxy.set_request_dispatcher(std::bind(&server_base::request_dispatch, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    while(!m_stop_flag) {
        m_ios.run();
    }
}

void server::request_dispatch(const char* req_data, std::size_t req_len, response_dispatch_type rsp_dispatcher) {
    SNOW_LOG_TRACE <<  "new request : " << req_data << std::endl;
    auto new_session = std::make_shared<session_type>(m_ios);
    new_session->set_response_dispatcher(std::move(rsp_dispatcher));
    new_session->start(req_data, req_len);
}