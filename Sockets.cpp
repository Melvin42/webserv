#include "Sockets.hpp"

/**** SOCKET ****/

Socket::Socket() {
}

Socket::Socket(const Socket &socket) {
	*this = socket;
	return ;
}

Socket::~Socket() {
}

Socket &Socket::operator=(const Socket &socket) {
	(void)socket;
	return *this;
}

/**** SOCKET CLIENT ****/

SocketClient::SocketClient(const std::string &host, int port) : Socket() {
	struct sockaddr_in	address;

	(void)host;
	(void)port;
	(void)address;
}

/**** SOCKET SERVER ****/

SocketServer::SocketServer(int port, int connections) : Socket() {
	struct sockaddr_in	address;

	memset(&address, 0, sizeof(address));

	if ((_server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
//		close(_server_fd);
		throw "INVALID SOCKET";
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);
	memset(address.sin_zero, 0, sizeof(address.sin_zero));

	if (bind(_server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
		close(_server_fd);
		throw "INVALID SOCKET";
	}
	if (listen(_server_fd, connections) < 0) {
		throw "INVALID SOCKET";
	}
}

int	SocketServer::Accept() {
	int	new_socket;
	int	addrlen = sizeof(_address);

	if ((new_socket = accept(_server_fd, (struct sockaddr *)&_address, (socklen_t *)&addrlen)) < 0) {
		throw "INVALID SOCKET";
		std::cerr << "Bad Accept" << std::endl;
	}

	return new_socket;
}
