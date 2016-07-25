//
// Created by weitao on 7/17/16.
//

#include "snow.h"

class echo : public snow::server
{
public:
    virtual std::size_t pkg_check(const char* data, std::size_t len)
    {
        return len;
    }

    virtual void request_dispatch(const char* req_data, std::size_t req_len, response_dispatch_type rsp_dispatcher)
    {

    }
};


int main(int argc, char* argv[])
{
    echo echo1;
    echo1.init("config.yaml");
    echo1.start();
    return 0;
}