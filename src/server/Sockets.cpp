#define BUFFER_SIZE 2048

#include "Sockets.hpp"

/**** SOCKET ****/

Socket::Socket() : _env(NULL) {
}

Socket::~Socket() {
	for (size_t i = 0; i < _server_fd.size(); i++) {
		close(_server_fd.at(i));
	}
}

int	Socket::getMasterFd(size_t id) const {
	return _server_fd.at(id);
}

std::vector<ClientManager>	&Socket::getClientSocket() {
	return _clientSocket;
}

void	Socket::addServerFd(int fd) {
	_server_fd.push_back(fd);
}

/**** SOCKET SERVER ****/

SocketServer::SocketServer(char **env, const Config &conf) : Socket() {
	_config = conf;
	_env = env;
	_sd = 0;
	_max_sd = 0;
}

void	SocketServer::bindSocket(const BlockConfig &block) {
	int					opt = true;
	int					tmp = 0;
	struct sockaddr_in	address;

	memset(&address, 0, sizeof(address));

	
	if ((tmp = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		throw "INVALID SOCKET";
	}
	this->addServerFd(tmp);

	std::cerr << "block id = " << block.getId() << std::endl;
	std::cerr << "block fd = " << tmp << std::endl;

	if (setsockopt(_server_fd.at(block.getId()), SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
			sizeof(opt)) < 0) {
		throw "INVALID SOCKET";
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(block.getPort());
	memset(address.sin_zero, 0, sizeof(address.sin_zero));

	if (bind(_server_fd.at(block.getId()), (struct sockaddr *)&address, sizeof(address)) < 0) {
		throw "INVALID SOCKET";
	}
	if (listen(_server_fd.at(block.getId()), 0) < 0) {
		throw "INVALID SOCKET";
	}
}

int	SocketServer::getSocketUsed() const {
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

void	SocketServer::setUpSocket(const BlockConfig &block, int connections) {
	int					opt = true;
	int					fd;
	struct sockaddr_in	address;

	memset(&address, 0, sizeof(address));

	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		throw "INVALID SOCKET";
	}
	_server_fd.push_back(fd);
	if (setsockopt(_server_fd.at(block.getId()), SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
			sizeof(opt)) < 0) {
		throw "INVALID SOCKET";
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(block.getPort());
	std::cerr << address.sin_port << std::endl;
	std::cerr << address.sin_addr.s_addr << std::endl;
	memset(address.sin_zero, 0, sizeof(address.sin_zero));

	if (bind(_server_fd.at(block.getId()), (struct sockaddr *)&address, sizeof(address)) < 0) {
		std::cerr << "VILAIN" << std::endl;
		perror("bind failed");
		throw "INVALID SOCKET";
	}
	if (listen(_server_fd.at(block.getId()), connections) < 0) {
		throw "INVALID SOCKET";
	}
}

int	SocketServer::acceptSocket(const BlockConfig &block) {
	int	new_socket;
	int	addrlen = sizeof(_address);

	if ((new_socket = accept(_server_fd.at(block.getId()), (struct sockaddr *)&_address, (socklen_t *)&addrlen)) < 0) {
		throw "INVALID SOCKET";
	}
	return new_socket;
}

void	SocketServer::selectSocket(const BlockConfig &block) {
	int											activity;
	std::vector<ClientManager>::iterator		it;
	std::vector<ClientManager>::const_iterator	ite = this->getClientSocket().end();
	int											server_fd = _server_fd.at(block.getId());

	FD_ZERO(&_readfds);
	FD_SET(_server_fd.at(block.getId()), &_readfds);
	FD_ZERO(&_writefds);
	FD_SET(_server_fd.at(block.getId()), &_writefds);
	_max_sd = _server_fd.at(block.getId());

	for (it = this->getClientSocket().begin(); it != ite; it++) {
		//if (it->getPort() == block.getPort())
		_sd = it->getFd();
		if (_sd > 0) {
			FD_SET(_sd, &_readfds);
			FD_SET(_sd, &_writefds);
		}
		if (_sd > _max_sd)
			_max_sd = _sd;
	}

	activity = select(_max_sd + 1, &_readfds, &_writefds, NULL, NULL);

	if ((activity < 0) && (errno != EINTR))
		throw "SELECT FAILED";
}

bool	SocketServer::ready(int fd, fd_set set) {
	if (FD_ISSET(fd, &set))
		return true;
	return false;
}

void	SocketServer::setClientSocket(const BlockConfig &block) {

	if (this->ready(this->getMasterFd(block.getId()), this->getReadFds())) {

		ClientManager	new_client(this->acceptSocket(block), block);

		this->getClientSocket().push_back(new_client);
		for (size_t i = 0; i < this->getClientSocket().size(); i++) {
			std::cerr << "fd = " << this->getClientSocket().at(i).getFd() << std::endl;
			std::cerr << "block id = " << this->getClientSocket().at(i).getBlock().getId() << std::endl;
			std::cerr << "block root  = " << this->getClientSocket().at(i).getBlock().getRoot() << std::endl;
			std::cerr << "block port = " << this->getClientSocket().at(i).getBlock().getPort() << std::endl;
		}
	}
}

void	SocketServer::simultaneousRead(const BlockConfig &block) {
	std::vector<ClientManager>::iterator		it;
	std::vector<ClientManager>::const_iterator	ite;
	char	buffer[BUFFER_SIZE + 1] = {0};

	ite = this->getClientSocket().end();
	std::string	str_file = "";
	for (it = this->getClientSocket().begin(); it != ite; it++) { //this loop is dedicated to every read fds ready for use 
		this->setSocketUsed(it->getFd());
		if (this->ready(this->getSocketUsed(), this->getReadFds())) { //here we check if the socket is ready for reading
			long	valread = 0;

			//here we read max BUFFER_SIZE (=2048) data for each sockets,
			if ((valread = recv(this->getSocketUsed(), buffer, BUFFER_SIZE, 0)) == 0) { //if read == 0 means client disconnect
				this->closeClean(&_readfds);
				it->setFd(0);
			} else if (valread < 0) { // if read < 0 is an error
				perror("recv failed");
				this->closeClean(&_readfds);
				it->setFd(0);
			} else { //here is what we do when the client send us a request
				it->appendRead(buffer); //we will append to ClientManager::_read as long as we haven't recv all the request from the client
				if (it->isReadOk()) { //this is where we check if we have all the request in ClientManager::_read
					HttpRequest	req(it->getRead().c_str(), it->getBlock());
					HttpResponse	msg(_env, req.getRequest());
					str_file = msg.getHttpResponse();
					it->setSend(str_file); //this is where the response is stored
					// std::cerr << "bytes to send: " << it->getSend().size() << std::endl;
					str_file = "";
					it->setRead("");
					it->setReadOk(false);
					it->setSendOk(true);
					//at this stage the response is already stored in ClientManager::_send
					//the response will be send in the second loop if the socket is ready for writing
				}
			}
		}
	}
	for (it = this->getClientSocket().begin(); it != ite; it++) {//this loop is dedicated to every write fds ready for use 
		this->setSocketUsed(it->getFd());
		if (this->ready(this->getSocketUsed(), this->getWriteFds()) && it->getSendOk()) {//here we check if the socket is ready for writing
			int	valsend = 0;

			//here we send in one time all the response (stored in ClientManager::_send) for each sockets,
			if ((valsend = send(this->getSocketUsed(), it->getSend().c_str(), it->getSend().size(), 0)) == -1) { //if send == -1 is an error
				std::cerr << "send failed: " << it->getSend().size() << std::endl;
			} else if (valsend < static_cast<int>(it->getSend().size())) { // if send < size means that we didn't send all the response
				std::string	msg = it->getSend().substr(0, valsend);
				//here i'm storing all datas that weren't send in order to try to send them later
				it->setSend(msg); //updating ClientManager::_send (puting only what we didn't already send)
			} else { //here is what we do if we had send all the response to the client
				this->closeClean(&_writefds);
				it->setSend("");
				it->setSendOk(false);
				it->setFd(0);
				//at this stage the request from the client should be satisfied
				//no data needs to be stored anymore (until a new request happend) and the socket need to be closed
			}
		}
	}
}

void	SocketServer::setUpBlockServer() {
	for (size_t i = 0; i < _config.getConfig().size(); i++)
		this->bindSocket(_config.getConfig().at(i));
}

void	SocketServer::run() {
	int	count_loop = 0;
	this->setUpBlockServer();
	while (true) {
		count_loop++;
		for (size_t i = 0; i < _config.getConfig().size(); i++) {
			this->selectSocket(_config.getConfig().at(i));
			this->setClientSocket(_config.getConfig().at(i));
		}
		this->simultaneousRead();
	}
}

void	SocketServer::closeClean(fd_set *fds) {
	close(_sd);
	FD_CLR(_sd, fds);
}
