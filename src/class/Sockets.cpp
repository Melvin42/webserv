#include "Sockets.hpp"

/**** SOCKET ****/

Socket::Socket(char **env) : _clientSocket(30, 0), _env(env) {
}

Socket::~Socket() {
	close(_server_fd);
}

int	Socket::getMasterFd() const {
	return _server_fd;
}

std::vector<int>	&Socket::getClientSocket() {
	return _clientSocket;
}

Socket &Socket::operator=(const Socket &socket) {
	(void)socket;
	return *this;
}

/**** SOCKET SERVER ****/

SocketServer::SocketServer(char **env, int port, int connections) : Socket(env) {
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

int	SocketServer::getSocketUsed() const {
//	std::cerr << '[' <<_sd << ']';
	return _sd;
}

void	SocketServer::setSocketUsed(int fd) {
	_sd = fd;
}

fd_set	SocketServer::getReadFds() const {
	return _readfds;
}

int	SocketServer::acceptSocket() {
	int	new_socket;
	int	addrlen = sizeof(_address);

	if ((new_socket = accept(_server_fd, (struct sockaddr *)&_address, (socklen_t *)&addrlen)) < 0) {
		throw "INVALID SOCKET";
	}
	return new_socket;
}

void	SocketServer::selectSocket() {
	int									activity;
	std::vector<int>::iterator			it;
	std::vector<int>::const_iterator	ite = this->getClientSocket().end();

	FD_ZERO(&_readfds);
	FD_SET(_server_fd, &_readfds);
	_max_sd = _server_fd;

	for (it = this->getClientSocket().begin(); it != ite; it++) {
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

bool	SocketServer::ready(int fd, fd_set set) {
	if (FD_ISSET(fd, &set))
		return true;
	return false;
}

void	SocketServer::setClientSocket() {
	std::vector<int>::iterator			it;
	std::vector<int>::const_iterator	ite;
	int									new_socket = 0;

	if (this->ready(this->getMasterFd(), this->getReadFds())) {
		new_socket = this->acceptSocket();
		std::cerr << "New connection, socket fd is " << new_socket << std::endl;
		for (it = this->getClientSocket().begin(); it != ite; it++) {
			if (*it == 0) {
				*it = new_socket;
				break ;
			}
		}
	}
}

void	SocketServer::simultaneousRead() {
	std::vector<int>::iterator			it;
	std::vector<int>::const_iterator	ite;
	char	buffer[BUFFER_SIZE + 1] = {0};
	long	valread = 0;

	ite = this->getClientSocket().end();
	std::string	str_file = "";
	for (it = this->getClientSocket().begin(); it != ite; it++) {
		this->setSocketUsed(*it);
		if (this->ready(this->getSocketUsed(), this->getReadFds())) {
			if ((valread = read(this->getSocketUsed(), buffer, BUFFER_SIZE)) == 0) {
				std::cout << "read = 0" << buffer  << std::endl;
				// maybe with POST: this->closeClean();
			} else {
				std::cout << "read = " << valread << " content:\n" << buffer << std::endl;
				HttpRequest	req(buffer, BUFFER_SIZE);
				HttpResponse	msg(_env);
				str_file = msg.getHttpResponse(req.getPage());
				if (send(this->getSocketUsed(), str_file.c_str(),
							str_file.size(), 0) == static_cast<long>(str_file.size())) {
					this->closeClean();
					*it = 0;
				}
			}
		}
	}
}

void	SocketServer::run() {
	int	count_loop = 0;
	while (true) {
		count_loop++;
		std::cout << "count_loop = " << count_loop << std::endl;
		this->selectSocket();
		std::cout << "\n+++++++ Waiting for new connection ++++++++\n" << std::endl;
		this->setClientSocket();
		this->simultaneousRead();
	}
}

void	SocketServer::closeClean() {
	close(_sd);
	FD_CLR(_sd, &_readfds);
}
