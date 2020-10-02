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

	Socket client(AF_UNIX, SOCK_DGRAM, 0, client_addr);

	std::cout << "waiting for message from server socket..." << std::endl;
	std::cout << client.receive_int(server_addr) << std::endl;
	std::cout << "sending to client socket..." << std::endl;
	client.send_int(server_addr, 2);
} catch (const std::exception& e) {
	std::cerr << e.what() << std::endl;
	return EXIT_FAILURE;
}
