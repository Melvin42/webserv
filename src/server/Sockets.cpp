#define BUFFER_SIZE 2048

#include "Sockets.hpp"

/**** SOCKET ****/

Socket::Socket() : _server_fd(0), _env(NULL), _clientSocket(1, 0) {
}

Socket::~Socket() {
	close(_server_fd);
}

int	Socket::getMasterFd() const {
	return _server_fd;
}

std::vector<ClientManager>	&Socket::getClientSocket() {
	return _clientSocket;
}

/**** SOCKET SERVER ****/

SocketServer::SocketServer(char **env, const Config &conf, int connections) : Socket() {
	int					opt = true;
	struct sockaddr_in	address;

	_config = conf;
	_env = env;
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
//	address.sin_port = htons(8080);
	address.sin_port = htons(conf.getConfig().at(0).getPort().at(0));
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

fd_set	SocketServer::getWriteFds() const {
	return _writefds;
}

Config	&SocketServer::getConfig() {
	return _config;
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
	std::vector<ClientManager>::iterator			it;
	std::vector<ClientManager>::const_iterator	ite = this->getClientSocket().end();

	FD_ZERO(&_readfds);
	FD_SET(_server_fd, &_readfds);
	FD_ZERO(&_writefds);
	FD_SET(_server_fd, &_writefds);
	_max_sd = _server_fd;
//
	for (it = this->getClientSocket().begin(); it != ite; it++) {
		_sd = it->getFd();
		if (_sd > 0) {
			FD_SET(_sd, &_readfds);
			FD_SET(_sd, &_writefds);
		}
		if (_sd > _max_sd)
			_max_sd = _sd;
	}

	activity = select(_max_sd + 1, &_readfds, &_writefds, NULL, NULL);

	if ((activity < 0) && (errno != EINTR))						//Don't use errno!!!
		throw "SELECT FAILED";
}

bool	SocketServer::ready(int fd, fd_set set) {
	if (FD_ISSET(fd, &set))
		return true;
	return false;
}

void	SocketServer::setClientSocket() {
//	std::vector<ClientManager>::iterator		it;
//	std::vector<ClientManager>::const_iterator	ite;
//	int									new_socket = 0;

	if (this->ready(this->getMasterFd(), this->getReadFds())) {
		ClientManager	new_client(this->acceptSocket());
		std::cerr << "New connection, socket fd is " << new_client.getFd() << std::endl;
		this->getClientSocket().push_back(new_client);
	}
}
//		new_socket = this->acceptSocket();
//		std::cerr << "New connection, socket fd is " << new_socket << std::endl;
//		for (it = this->getClientSocket().begin(); it != ite; it++) {
//			if (*it == 0) {
//				it->setFd(new_socket);
//				*it = new_socket;
//				break ;
//			}
//		}

void	SocketServer::simultaneousRead() {
	std::vector<ClientManager>::iterator			it;
	std::vector<ClientManager>::const_iterator	ite;
	char	buffer[BUFFER_SIZE + 1] = {0};
	long	valread = 0;

	ite = this->getClientSocket().end();
	std::string	str_file = "";
	for (it = this->getClientSocket().begin(); it != ite; it++) {
		this->setSocketUsed(it->getFd());
		if (this->ready(this->getSocketUsed(), this->getReadFds())) {
			std::cerr << "read fd ready" << std::endl;
			std::cout << "+++++++receiving data to client++++++++" << std::endl;
			if ((valread = read(this->getSocketUsed(), buffer, BUFFER_SIZE)) <= 0) {
				std::cout << "valread <= 0 fd_used = " << it->getFd() << std::endl;
				this->closeClean();
//				it->setRead("");
				it->setFd(0);
//				it->setReadOk(false);
			} else {
				std::cerr << "valread = " << valread
				   << "fd_used = " << it->getFd() << std::endl;
				it->appendRead(buffer);
				if (it->isReadOk(this->getConfig().getPath())) {
					HttpRequest	req(it->getRead().c_str(),
							it->getRead().size(), this->getConfig().getPath());
					HttpResponse	msg(_env);
					str_file = msg.getHttpResponse(req.getPage());
//					it->setSend(msg.getHttpResponse(req.getPage()));
					it->setRead("");
//					it->setSendOk(false);
					it->setReadOk(false);
					if (this->ready(this->getSocketUsed(), this->getWriteFds())) {
						std::cerr << "write fd ready" << std::endl;
						std::cout << "+++++++sending data to client++++++++" << std::endl;
						if (send(this->getSocketUsed(), str_file.c_str(),
								str_file.size(), 0) == static_cast<ssize_t>(str_file.size())) {
							this->closeClean();
							it->setFd(0);
//						if (send(this->getSocketUsed(), it->getSend().c_str(),
//								it->getSend().size(), 0) == static_cast<long>(it->getSend().size())) {
//							it->setSendOk(true);
//							it->setSend("");
						} else {
							std::cerr << "send failed: " << it->getSend() << std::endl;
						}
					}
				}
			}
		}
	}
}

					///////////////////////////////////////////////
					
void	SocketServer::run() {
	int	count_loop = 0;
	while (true) {
		count_loop++;
//		std::cout << "count_loop = " << count_loop << std::endl;
		this->selectSocket();
//		std::cout << "\n+++++++ Waiting for new connection ++++++++\n" << std::endl;
		this->setClientSocket();
		this->simultaneousRead();
	}
}

void	SocketServer::closeClean() {
	close(_sd);
	FD_CLR(_sd, &_readfds);
}
