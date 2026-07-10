#pragma once
#include "Socket_base.h"
#include "Session_unit.h"

class Socket_server : public Socket_base
{

public:
	//constructors
	Socket_server(std::string port = "21801");
	~Socket_server();


	Socket_server(const Socket_server&) = delete;
	Socket_server& operator=(const Socket_server&) = delete;

	Socket_server(Socket_server&&) = delete;
	Socket_server& operator=(Socket_server&&) = delete;

    //socket methods
	void bind_socket();
	void listen_socket();
	void accept_many_sockets();

	void shutdown_socket();

	void broadcast(std::string, SOCKET);
	void add_session(SOCKET socket);
	void start();
	void cleanup_sessions();

private:

	//for only one client
	std::jthread  accept_thread;

	std::mutex mtx;

	//std::jthread m_recv_thread;
	//std::jthread m_send_thread;


	//for many clients
	std::vector<std::unique_ptr<Session_unit>> sessions;

};



