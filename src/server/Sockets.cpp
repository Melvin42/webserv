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
	_max_sd = _server_fd;
//
	for (it = this->getClientSocket().begin(); it != ite; it++) {
		_sd = it->getFd();
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
//	std::vector<ClientManager>::iterator		it;
//	std::vector<ClientManager>::const_iterator	ite;
//	int									new_socket = 0;

	if (this->ready(this->getMasterFd(), this->getReadFds())) {
		ClientManager	new_client(this->acceptSocket());
		// std::cerr  << std::endl << "++++++++++++++++" << std::endl
		// << "++++++++++++++++" << std::endl << std::endl
		std::cerr << "New connection, socket fd is " << new_client.getFd() << std::endl;
		this->getClientSocket().push_back(new_client);
//		new_socket = this->acceptSocket();
//		std::cerr << "New connection, socket fd is " << new_socket << std::endl;
//		for (it = this->getClientSocket().begin(); it != ite; it++) {
//			if (*it == 0) {
//				it->setFd(new_socket);
//				*it = new_socket;
//				break ;
//			}
//		}
	}
}

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
			if ((valread = read(this->getSocketUsed(), buffer, BUFFER_SIZE)) == 0) {
//				std::cout << "valread = 0 fd_used = " << it->getFd() << std::endl;
				if (it->getSendOk() && it->getFd() > 2) {
					it->setSendOk(false);
					this->closeClean();
					it->setRead("");
					it->setFd(0);
					it->setReadOk(false);
				}
//				std::cout << "read = 0" << buffer  << std::endl;
				// maybe with POST: this->closeClean();
			} else {
				it->appendRead(buffer);
//					std::cout << "read = " << valread
//						<< " content:\n" << it->getRead() << std::endl;
//					std::cout << "++++++++received from client " << it->getFd()
//					   << "++++++++++++" << std::endl;
				if (it->isReadOk()) {
//					std::cout << "read OK, val = " << valread
//						<< " content:\n" << it->getRead() << std::endl;
					// std::cout << "something here" << std::endl;
					HttpRequest	req(it->getRead().c_str(), _config.getPath());
					// std::cout << std::endl << "------ method " << req.getMethod() << " ------" << std::endl;
					// std::cout << std::endl << "------request done------" << std::endl << std::endl;
					//					std::cerr << _config.getPath() << std::endl;
					std::cerr << req.getPage() << std::endl;
					HttpResponse	msg(_env, _config, req.getRequest());
					str_file = msg.getHttpResponse();
					// std::cout << std::endl << "------response done------" << std::endl << std::endl;
					
//					std::cerr << "BEFORE SEND =" << str_file << std::endl;
//					std::cout << "+++++++sending data to client++++++++" << std::endl;
					// std::cout << "SendOk: " << it->getSendOk() << std::endl;
					// std::cout << "str_file.size(): " << str_file.size() << std::endl;
					int retsend = 0;
					if (it->getSendOk() == false 
							&& (retsend = send(this->getSocketUsed(), str_file.c_str(),
							str_file.size(), 0)) == static_cast<long>(str_file.size())) {
						it->setSendOk(true);
						it->setSend(str_file);
						this->closeClean();
						it->setRead("");
						it->setFd(0);
						it->setReadOk(false);
//						*it = 0;
						// std::cout << "------client fd closed------" << std::endl;
					}
					// std::cout << "retsend        : " << retsend << std::endl;
					// std::cout << "------page sent------" << std::endl;
				}
			}
		}
	}
}

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
