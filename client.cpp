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

	std::cout << "waiting for server..." << std::endl;
	auto text_files_count = client.receive<int>(server_addr);
	std::cout << "Text files: " << text_files_count << std::endl;
	std::cout << "sending to server..." << std::endl;
	client.send(server_addr, text_files_count - read_pipe<int>("ls | file --mime-type -bf - | grep -c 'text/x-shellscript'"));
} catch (const std::exception& e) {
	std::cerr << e.what() << std::endl;
	return EXIT_FAILURE;
}
