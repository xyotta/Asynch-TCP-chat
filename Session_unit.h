#pragma once
#include "pch.h"
using BroadcastFunction = std::function<void(std::string, SOCKET)>;

struct Session_unit {
public:
	Session_unit(SOCKET socket, BroadcastFunction reciving_msg) :
		session_socket(socket),
		session_id("guy-" + std::to_string(++session_count)), //guy-1, guy-2, guy-3...
		broadcast(reciving_msg)
	{
		std::cout << "Session " << session_count << " added to queque" << std::endl;
		++session_active;
		session_thread = std::jthread([this]() {

			int result;
			char buf[512];
			while (true) {
				result = recv(session_socket, buf, sizeof(buf), 0);
				if (result > 0) {
					std::string msg(buf, result);
					std::cout << session_id << ": " << msg << std::endl;
					broadcast((session_id+":" + msg), session_socket);
				}
				else if (result < 0) {
					std::cout << session_id << " left this party" << std::endl;
					break;
				}
			}
			is_dead = true;
			});
	}

	~Session_unit() {
		if (session_socket != INVALID_SOCKET) {
			closesocket(session_socket);
			--session_active;
		}
		std::cout << "Session " << session_id << " closed. active sessions: " << session_active << std::endl;
	}

	Session_unit(const Session_unit&) = delete;
	Session_unit& operator=(const Session_unit&) = delete;
	Session_unit(Session_unit&&) = delete;
	Session_unit& operator=(Session_unit&&) = delete;

	int get_session_count() const { return session_count; }
	int get_session_active() const { return session_active; }




	inline static std::atomic_int session_count = 0;
	inline static std::atomic_int session_active = 0;
	std::atomic_bool is_dead = false;
	std::string session_id;
	SOCKET session_socket = INVALID_SOCKET;
	BroadcastFunction broadcast;
	std::jthread session_thread;
};