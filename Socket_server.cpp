#include "Socket_server.h"


// Constructor/Destructor
Socket_server::Socket_server(std::string port) : Socket_base("",port)
{
	std::cout << "Initializing server socket on port: " << port << std::endl;
	bind_socket();
	listen_socket();
}

Socket_server::~Socket_server()
{
	accept_thread.request_stop();

	if (socket_fd != INVALID_SOCKET) {
		closesocket(socket_fd);
	};

	if (accept_thread.joinable())
	{
		accept_thread.join();
	}

	shutdown_socket();
}
void Socket_server::bind_socket()
{
	if (int ISresult = bind(socket_fd,
							result->ai_addr,
							(int)result->ai_addrlen); ISresult == SOCKET_ERROR)
	{
		std::cout << "Invalid address in bind_socket() " << ISresult << std::endl;
		throw std::runtime_error("failed to bind socket");
	}
	std::cout << "Socket bound to port successfully" << std::endl;

}
void Socket_server::listen_socket()
{
	if (int ISresult = listen(socket_fd, SOMAXCONN); ISresult == SOCKET_ERROR)
	{
		std::cout << "Listen failed with error: " << WSAGetLastError() << std::endl;
		throw std::runtime_error("failed to listen on socket");
	}
	std::cout << "Listening on socket..." << std::endl;
}

void Socket_server::shutdown_socket() {


	std::unique_lock<std::mutex> lock(mtx);
	for (auto& session : sessions) {
		SOCKET Client_socket = session->session_socket;
		if (Client_socket == INVALID_SOCKET)
		{
			std::cout << "сlient socket isnt init" << std::endl;
		}
		else {
			if (int IsResult = shutdown(Client_socket, SD_SEND); IsResult == SOCKET_ERROR)
			{
				std::cout << "Shutdown failed with error: " << WSAGetLastError() << std::endl;
				continue;
			}
		}
	}

}


//accept only one socket

void Socket_server::accept_many_sockets()
{

	std::unique_lock<std::mutex> lock(mtx,std::defer_lock);
	while (true) {
		SOCKET client_socket = accept(socket_fd, NULL, NULL);

		if (client_socket == INVALID_SOCKET)
		{
			int errcode = WSAGetLastError();
			if (errcode == WSAEINTR || errcode == WSAENOTSOCK) {
				std::cout << "Acccept failed while server proceded shutdown" << std::endl;
				break;
			}
			std::cout << "Accept failed with error: " << errcode << std::endl;
			break;
		}
		std::cout << "Client connected successfully" << std::endl;
		std::cout << "Client socket: " << client_socket << std::endl;
		lock.lock();
		cleanup_sessions();
		add_session(client_socket);
		lock.unlock();

	}
}

void Socket_server::start() {
	accept_thread = std::jthread([this]() {
		accept_many_sockets();
		});
}

//accept many sockets

void Socket_server::add_session(SOCKET socket) {
	sessions.push_back(std::make_unique<Session_unit>(
		socket,
		[this](std::string msg, SOCKET sender) {
			broadcast(msg, sender);
		}
	));
	std::cout << "Session added. Total sessions: " << sessions.size() << std::endl;
}


void Socket_server::broadcast(std::string msg, SOCKET sender) {
	std::lock_guard<std::mutex> lock(mtx);
	for (auto& session : sessions) {
		if (session->session_socket != sender) {
			send(session->session_socket, msg.c_str(),(int)msg.size(), 0);
		}
	}
}

void Socket_server::cleanup_sessions() {
	sessions.erase(
		std::remove_if(sessions.begin(), sessions.end(),
			[](const auto& s) { return s->is_dead.load(); }),
		sessions.end()
	);
}



