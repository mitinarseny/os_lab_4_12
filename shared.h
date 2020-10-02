#ifndef SHARED_H
#define SHARED_H

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <string>

const std::string SERVER_SOCK_PATH = "/tmp/lab4_server";
const std::string CLIENT_SOCK_PATH = "/tmp/lab4_client";

class Socket {
public:
	Socket(int domain, int type, int protocol, const sockaddr_un& sa);
	~Socket();
	
	void send_int(const sockaddr_un& sa, int val);
	int receive_int(sockaddr_un& sa);
protected:
	int soc;
	sockaddr_un addr;
};

#endif // SHARED_H
