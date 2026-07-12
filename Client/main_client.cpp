#include "pch.h"

int main() {
	std::string port,host;
	std::cout << "--CLIENT--" << std::endl;

	std::cout << "Enter host (or press Enter for default ::1" << std::endl;
	std::getline(std::cin, host);


	std::cout << "Enter port (or press Enter for default 21801" << std::endl;
	std::getline(std::cin, port);




	if (port.empty()) {
		port = "21801";
	}
	if (host.empty()) {
		port = "::1";
	}


	try {
		Socket_client client(host,port);
		client.start();
		std::getline(std::cin, port);

	}
	catch (std::exception& e) {
		std::cout << "Error: " << e.what() << std::endl;
	}
	return 0;
}