#include "pch.h"
#pragma comment(lib, "Ws2_32.lib")
#include "Socket_server.h"
#include "Socket_client.h"

void menu(std::string& fist, std::string& second) {
    char pushed;
startover:
    std::cout << "Press 0 to default value" << std::endl;
    std::cout << "Press 1 to own value" << std::endl;
    while(true){
        std::cin >> pushed;
    switch (pushed) {
    case '0' :
        std::cout << "ok" << std::endl;
        return;

    case '1':
        std::cout << "enter host, press enter if server" << std::endl;
        std::cin.ignore();
        std::getline(std::cin, fist);
        std::cout << "enter port" << std::endl;
        std::cin >> second;
        if (fist.empty()) fist = "::1";
        if (second.empty()) second = "21801";
        return;

    default:
        std::cout << "not matched, pls chose another option" << std::endl;
    }

    }
}


int main(int argc, char* argv[]) {
    std::string host, port;
    try {
        std::string arg = argv[1];
        if (arg == "server") {
            menu(host, port);
            Socket_server server(port);
            server.start();
            std::cin.ignore();
            std::getline(std::cin, host);
        }
        else if (arg == "client") {
            menu(host, port);
            Socket_client client(host,port);
            client.start();
        }
    }
    catch (std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        std::cin.get();
    }
    return 0;
}