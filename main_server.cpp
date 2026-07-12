#include "pch.h"
#include "Socket_server.h"

int main() {
	std::string port;
	std::cout << "--SERVER--" << std::endl;
	std::cout << "Enter port (or press Enter for default 21801" << std::endl;
	std::getline(std::cin, port);

	if (port.empty()) {
		port = "21801";
	}


	try {
		Socket_server server(port);
		server.start();
		std::cout << "Press button to shutdown server" << std::endl;
		std::getline(std::cin, port);

	}
	catch (std::exception& e) {
		std::cout << "Error: " << e.what() << std::endl;
	}
	return 0;
}