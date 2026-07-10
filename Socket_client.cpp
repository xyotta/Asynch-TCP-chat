#include "Socket_client.h"

Socket_client::Socket_client(std::string host, std::string port) : Socket_base(host,port)  {

	std::cout << "Inisializing Client" << std::endl;
	while (connect(socket_fd,
		result->ai_addr,
		(int)result->ai_addrlen) == SOCKET_ERROR) {
		if (WSAGetLastError() != WSAECONNREFUSED) {
			std::cout << "unable to connect" << std::endl;
			throw std::runtime_error("connection failed" + std::to_string(WSAGetLastError()));
		}
		std::cout << "server not ready, retrying.." << std::endl;
		Sleep(1000);
		closesocket(socket_fd);
		init_socket();
	}
}


Socket_client::~Socket_client() {
	disconnect_socket();
}

void Socket_client::disconnect_socket() {
	if (int Result = shutdown(socket_fd, SD_SEND); Result == SOCKET_ERROR) {
		std::cout << "shutdown failed" << WSAGetLastError() << std::endl;
	}
}

void Socket_client::start() {
	std::jthread recv_thread([this]() {
		char buf[512];
		int result;
		while (true) {
			result = recv(socket_fd, buf, sizeof(buf), 0);
			if (result > 0) {
				std::string msg(buf, result);
				std::cout << "\n" << msg << std::endl;
			}
			else {
				std::cout << "server disconnected" << std::endl;
				break;
			}
		}
		});
	std::string msg;
	while (true) {

		std::getline(std::cin, msg);
		if (msg == "exit") break;
		std::cout << "you: " << msg << std::endl;
		if (int result = send(socket_fd, msg.c_str(), msg.size(), 0); result == SOCKET_ERROR) {
			std::cout << "server discontinued" << std::endl;
			break;
		}

	}
}