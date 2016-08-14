//
// Created by weitao on 7/17/16.
//

#include <iostream>
#include "snow.h"

class echo_session : public snow::session
{
public:
    echo_session() = default;

    virtual void process(const char* data, std::size_t len) {
//        std::cout << data << std::endl;
        send_to_client(data, len);
    }

};

class echo : public snow::server_base<echo_session>
{
public:
    virtual std::size_t pkg_check(const char* data, std::size_t len)
    {
//        std::cout << "len:" << len << std::endl;
        return len;
    }

};


int main(int argc, char* argv[])
{
    echo echo1;
    echo1.init("config.yaml");
    echo1.start();
    return 0;
}