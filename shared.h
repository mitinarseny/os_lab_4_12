#ifndef SHARED_H
#define SHARED_H

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <unistd.h>

#include <string>
#include <sstream>
#include <system_error>

const std::string SERVER_SOCK_PATH = "/tmp/lab4_server";
const std::string CLIENT_SOCK_PATH = "/tmp/lab4_client";

class Socket {
public:
	Socket(int domain, int type, int protocol, const sockaddr_un& sa);
	~Socket();
	
	template<typename T>
	void send(const sockaddr_un& sa, T val) {
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

	template<typename T>
	T receive(sockaddr_un& sa) {
		T val;
		socklen_t addr_len = sizeof(sa);
		if (::recvfrom(this->soc, reinterpret_cast<void*>(&val), sizeof(val), 0, 
			reinterpret_cast<sockaddr*>(&sa), &addr_len) == -1)
			throw std::system_error(errno, std::system_category(), "recvfrom");
		return val;
	}
protected:
	int soc;
	sockaddr_un addr;
};

struct piped_file_closer {
	void operator ()(std::FILE* fp) const;
};

using unique_FILE = std::unique_ptr<std::FILE, piped_file_closer>;

unique_FILE open_piped_command(std::string const& cmd, char const* mode);

std::string piped_read(std::string const& cmd);

template<typename T>
T read_pipe(std::string const& cmd) {
	std::istringstream o(piped_read(cmd));
	T val;
	o >> val;
	return val;
}

#endif // SHARED_H
