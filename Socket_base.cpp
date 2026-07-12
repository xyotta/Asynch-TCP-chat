#include "pch.h"
#include "Socket_base.h"
Socket_base::Socket_base(std::string host, std::string port) {
	//init WSA

	if (int ISresult = WSAStartup(MAKEWORD(2, 2), &wsaData); ISresult != 0) {
		std::cout << "WSA startup failed :( " << ISresult << std::endl;
		WSACleanup();
		throw std::runtime_error("WSA startup failed");
	}

	//init addrinfo

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	


	const char* possible_addr = host.empty() ? [&]() { hints.ai_flags = AI_PASSIVE; return nullptr; }() : host.c_str();

	if (int ISresult = getaddrinfo(possible_addr, port.c_str(), &hints, &result); ISresult != 0) {
		std::cout << "failed to get addr info" << ISresult << std::endl;
		WSACleanup();
		throw std::runtime_error("failed to get addr info");
	}

	//init socket

	init_socket();
}
	
Socket_base::~Socket_base() {
	if (socket_fd != INVALID_SOCKET) {
		closesocket(socket_fd);
		--socket_count;
	}
	freeaddrinfo(result);

	if (socket_count == 0) { 
		WSACleanup();
	}
}


void Socket_base::init_socket() {

	if(socket_fd == INVALID_SOCKET)
	{
		socket_fd = socket(result->ai_family,
			result->ai_socktype,
			result->ai_protocol);
		if (socket_fd == INVALID_SOCKET) {
			std::cout << "Invalid socket #" << socket_fd << std::endl;
			throw std::runtime_error("failed initialize socket");
		}

		else ++socket_count;
	}
	else {
		std::cout << "Socket already initialized" << std::endl;
	}
}