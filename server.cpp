#include "shared.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

#include <iostream>
#include <exception>

int main() try {
	sockaddr_un server_addr{
		.sun_family = AF_UNIX,
	};
	strcpy(server_addr.sun_path, SERVER_SOCK_PATH.c_str());

	sockaddr_un client_addr{
		.sun_family = AF_UNIX,
	};
	strcpy(client_addr.sun_path, CLIENT_SOCK_PATH.c_str());

	Socket server(AF_UNIX, SOCK_DGRAM, 0, server_addr);

	std::cout << "sending to server socket..." << std::endl;
	server.send_int(client_addr, 1);
	std::cout << "waiting for message from client socket..." << std::endl;
	std::cout << server.receive_int(client_addr) << std::endl;
} catch (const std::exception& e) {
	std::cerr << e.what() << std::endl;
	return EXIT_FAILURE;
}
