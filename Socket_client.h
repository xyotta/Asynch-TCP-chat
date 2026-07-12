#pragma once
#include "Socket_base.h"
class Socket_client :
    public Socket_base
{
public:

    Socket_client(std::string host= "::1", std::string port = "21801");
    ~Socket_client();
    void disconnect_socket();
    void start();

};





