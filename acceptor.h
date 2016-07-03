//
// Created by weitao on 7/1/16.
//

#ifndef SNOW_LIBEV_ACCEPTOR_H
#define SNOW_LIBEV_ACCEPTOR_H

#include <cstdint>
#include <string>

struct ev_loop;
struct ev_io;
struct ev_timer;

class acceptor
{
public:
    acceptor(ev_loop* loop);

    acceptor(ev_loop* loop, const std::string& ip, uint16_t port);

    ~acceptor();

    static void handle_now_connection(ev_loop* loop, ev_io* io_watcher, int revents);

private:
    ev_loop* m_loop;
    ev_io*   m_io_watcher;
    std::string m_ip;
    uint16_t    m_port;
};


#endif //SNOW_LIBEV_ACCEPTOR_H
