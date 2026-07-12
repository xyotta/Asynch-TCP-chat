#include "pch.h"
#include "Socket_client.h"

int main() {
	std::string host, port;
	std::cout << "--CLIENT--" << std::endl;

	std::cout << "Enter hosr (or press Enter to default ::1" << std::endl;
	std::getline(std::cin, host);

	std::cout << "Enter port (or press Enter for 21801" << std::endl;
	std::getline(std::cin, port);
	
	if (host.empty()) host = "::1";
	if (port.empty()) port = "21801";
	
	try {
		Socket_client client(host, port);
		client.start();

	}
	catch(std::exception& e) {
		std::cout << "Error: " << e.what() << std::endl;
		std::cin.get();
	}
	return 0;

}