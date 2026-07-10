#pragma once
#include "pch.h"
class Socket_base
{
public:
	Socket_base(std::string host = "", std::string port = "21801");
	
	virtual ~Socket_base();

	Socket_base(const Socket_base&) = delete;
	Socket_base& operator=(const Socket_base&) = delete;

	Socket_base(Socket_base&&) = delete;
	Socket_base& operator=(Socket_base&&) = delete;
	
	void init_socket();



	
	
private:
	inline static WSADATA wsaData;
	inline static int socket_count = 0;
protected:
	addrinfo* result, hints;
	SOCKET socket_fd=INVALID_SOCKET;
	int  iResult,isendResult;
	
};
