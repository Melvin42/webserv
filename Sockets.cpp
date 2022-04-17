#include "Sockets.hpp"

/**** SOCKET ****/

Socket::Socket() : _clientSocket(30, 0) {
}

Socket::Socket(const Socket &socket) {
	*this = socket;
	return ;
}

Socket::~Socket() {
	close(_server_fd);
}

int	Socket::GetMasterFd() {
	return _server_fd;
}

std::vector<int>	&Socket::GetClientSocket() {
	return _clientSocket;
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
	int					opt = true;
	struct sockaddr_in	address;

	memset(&address, 0, sizeof(address));

	if ((_server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		throw "INVALID SOCKET";
	}

	if (setsockopt(_server_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
			sizeof(opt)) < 0) {
		throw "INVALID SOCKET";
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);
	memset(address.sin_zero, 0, sizeof(address.sin_zero));

	if (bind(_server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
		throw "INVALID SOCKET";
	}
	if (listen(_server_fd, connections) < 0) {
		throw "INVALID SOCKET";
	}
}

int	SocketServer::GetSocketUsed() const {
//	std::cerr << '[' <<_sd << ']';
	return _sd;
}

void	SocketServer::SetSocketUsed(int fd) {
	_sd = fd;
}

fd_set	SocketServer::GetReadFds() const {
	return _readfds;
}

int	SocketServer::Accept() {
	int	new_socket;
	int	addrlen = sizeof(_address);

	if ((new_socket = accept(_server_fd, (struct sockaddr *)&_address, (socklen_t *)&addrlen)) < 0) {
		throw "INVALID SOCKET";
	}
	return new_socket;
}

void	SocketServer::Select() {
	int									activity;
	std::vector<int>::iterator			it;
	std::vector<int>::const_iterator	ite = this->GetClientSocket().end();

	FD_ZERO(&_readfds);
	FD_SET(_server_fd, &_readfds);
	_max_sd = _server_fd;

	for (it = this->GetClientSocket().begin(); it != ite; it++) {
		_sd = *it;
		if (_sd > 0)
			FD_SET(_sd, &_readfds);
		if (_sd > _max_sd)
			_max_sd = _sd;
	}

	activity = select(_max_sd + 1, &_readfds, NULL, NULL, NULL);

	if ((activity < 0) && (errno != EINTR))						//Don't use errno!!!
		throw "SELECT FAILED";
}

bool	SocketServer::Ready(int fd, fd_set set) {
	if (FD_ISSET(fd, &set))
		return true;
	return false;
}

void	SocketServer::CloseClean() {
	close(_sd);
	FD_CLR(_sd, &_readfds);
}
