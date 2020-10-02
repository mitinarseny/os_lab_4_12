#include "shared.h"

#include <unistd.h>
#include <errno.h>

#include <system_error>

int create_socket(int domain, int type, int protocol) {
	int soc = ::socket(domain, type, protocol);
	if (soc == -1)
		throw std::system_error(errno, std::system_category(), "socket");
	return soc;
}

void bind_socket(int socket, const sockaddr_un& sa) {
	if (::bind(socket, reinterpret_cast<const sockaddr*>(&sa), sizeof(sa)) == -1)
		throw std::system_error(errno, std::system_category(), "bind");
}

void unlink_socket(const char* path) {
	if (::unlink(path) == -1)
		throw std::system_error(errno, std::system_category(), "unlink");
}

void close_socket(int sfd) {
	while(::close(sfd) == -1) {
		if (errno == EINTR)
			continue;
		throw std::system_error(errno, std::system_category(), "close");
	}
}

Socket::Socket(int domain, int type, int protocol, const sockaddr_un& sa):
	soc(create_socket(domain, type, protocol)),
	addr(sa) {
	bind_socket(this->soc, this->addr);
}

Socket::~Socket() {
	unlink_socket(this->addr.sun_path);
	close_socket(this->soc);
}

void Socket::send_int(const sockaddr_un& sa, int val) {
	while (::sendto(this->soc, reinterpret_cast<const void*>(&val), sizeof(val), 0,
				reinterpret_cast<const sockaddr*>(&sa), sizeof(sa)) == -1) {
		switch (errno) {
		case ENOENT:
		case EINTR:
		case ECONNREFUSED:
			::usleep(100);
			continue;
		default:
			throw std::system_error(errno, std::system_category(), "sendto");
		}
	}
}

int Socket::receive_int(sockaddr_un& sa) {
	int val;
	socklen_t addr_len = sizeof(sa);
	if (::recvfrom(this->soc, reinterpret_cast<void*>(&val), sizeof(val), 0, 
		reinterpret_cast<sockaddr*>(&sa), &addr_len) == -1)
		throw std::system_error(errno, std::system_category(), "recvfrom");
	return val;
}
