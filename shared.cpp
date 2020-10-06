#include "shared.h"

#include <stdio.h>

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

void piped_file_closer::operator()(std::FILE* fp) const {
	::pclose(fp);
}

unique_FILE open_piped_command(std::string const& cmd, char const* mode) {
	std::FILE* p = ::popen(cmd.c_str(), mode);
	if (p == nullptr)
		throw std::system_error(errno, std::system_category(), "popen");
	return unique_FILE{p};
}

std::string piped_read(std::string const& cmd) {
	unique_FILE f = open_piped_command(cmd, "r");
	std::string output;
	char buf[512];
	while (auto len = std::fread(buf, sizeof(char), sizeof(buf), f.get()))
		output.append(buf, len);
	
	if (std::ferror(f.get()))
		throw std::system_error(errno, std::system_category(), "ferror");
	return output;
}
