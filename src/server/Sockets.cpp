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


const char	*SocketServer::SelectException::what(void) const throw() {
	return "Select failed";
}

const char	*SocketServer::InvalidSocketException::what(void) const throw() {
	return "Invalid socket";
}

bool	SocketServer::bindSocket(const BlockConfig &block) {
	int					opt = true;
	int					tmp = 0;
	struct sockaddr_in	address;

	memset(&address, 0, sizeof(address));

	if ((tmp = socket(AF_INET, SOCK_STREAM, 0)) == 0)
		return true;
	this->addServerFd(tmp);

	if (setsockopt(_server_fd.at(block.getId()), SOL_SOCKET, SO_REUSEADDR,
				&opt, sizeof(opt)) < 0)
		return true;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(block.getPort());
	memset(address.sin_zero, 0, sizeof(address.sin_zero));

	if (bind(_server_fd.at(block.getId()), (struct sockaddr *)&address,
			sizeof(address)) < 0)
		return true;
	if (listen(_server_fd.at(block.getId()), 0) < 0)
		return true;
	return false;
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

int	SocketServer::acceptSocket(const BlockConfig &block) {
	int	new_socket;
	int	addrlen = sizeof(_address);

	if ((new_socket = accept(_server_fd.at(block.getId()),
					(struct sockaddr *)&_address, (socklen_t *)&addrlen)) < 0) {
		throw(InvalidSocketException());
//		throw "INVALID SOCKET";
	}
	return new_socket;
}

void	SocketServer::FdZero() {
	FD_ZERO(&_readfds);
	FD_ZERO(&_writefds);
}

void	SocketServer::setFdSet(const BlockConfig &block) {
	FD_SET(_server_fd.at(block.getId()), &_readfds);
	FD_SET(_server_fd.at(block.getId()), &_writefds);
	_max_sd = _server_fd.at(block.getId());
}

void	SocketServer::selectSocket() {
	struct timeval								timeout;
	int											activity = 0;
	std::vector<ClientManager>::iterator		it;
	std::vector<ClientManager>::const_iterator	ite = this->getClientSocket().end();

	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
	for (it = this->getClientSocket().begin(); it != ite; it++) {
		_sd = it->getFd();
		if (_sd > 0) {
			FD_SET(_sd, &_readfds);
			FD_SET(_sd, &_writefds);
		}
		if (_sd > _max_sd)
			_max_sd = _sd;
	}

	activity = select(_max_sd + 1, &_readfds, &_writefds, NULL, &timeout);

	if ((activity < 0) && (errno != EINTR))
		throw(SelectException());
//		throw "SELECT FAILED";
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
	}
}

void	SocketServer::simultaneousRead() {
	std::vector<ClientManager>::iterator		it;
	std::vector<ClientManager>::iterator		ite;
	char	buffer[BUFFER_SIZE + 1] = {0};

	ite = this->getClientSocket().end();
	for (it = this->getClientSocket().begin(); it != ite; it++) {				 //this loop is dedicated to every read fds ready for use 
		for (int j = 0; j < BUFFER_SIZE + 1; j++) {
			buffer[j] = 0;
		}

		this->setSocketUsed(it->getFd());
		if (this->ready(this->getSocketUsed(), this->getReadFds())) {			 //here we check if the socket is ready for reading
			long	valread = 0;

																				//here we read max BUFFER_SIZE (=2048) data for each sockets,
			if ((valread = recv(this->getSocketUsed(), buffer,
							BUFFER_SIZE, 0)) == 0) {							 //if read == 0 means client disconnect
				it->setReadOk(true);
			} else if (valread < 0) {											 // if read < 0 is an error
				it->setReadOk(true);
			} else {															 //here is what we do when the client send us a request
				it->appendRead(buffer);											 //we will append to ClientManager::_read as long as we haven't recv all the request from the client
				if (it->isReadOk()) {											 //this is where we check if we have all the request in ClientManager::_read
					HttpRequest		req(it->getRead().c_str(), it->getBlock());
					HttpResponse	msg(_env, it->getBlock(), req.getRequest());
					it->setSend(msg.getHttpResponse());										 //this is where the response is stored
					it->setRead("");
																				 //at this stage the response is already stored in ClientManager::_send
																				 //the response will be send in the second loop if the socket is ready for writing
				}
			}
		}
	}
}

void	SocketServer::simultaneousWrite() {
	std::vector<ClientManager>::iterator		it;
	std::vector<ClientManager>::iterator		ite;

	ite = this->getClientSocket().end();

	for (it = this->getClientSocket().begin(); it != ite; it++) {				 //this loop is dedicated to every write fds ready for use 

		this->setSocketUsed(it->getFd());

		if (this->ready(this->getSocketUsed(), this->getWriteFds())) {

			if (it->getReadOk() && it->getSend() != "") {											 //here we check if the socket is ready for writing

				int	valsend = 0;
																					 //here we send in one time all the response (stored in ClientManager::_send) for each sockets,
				if ((valsend = send(this->getSocketUsed(), it->getSend().c_str(),
								it->getSend().size(), 0)) < static_cast<int>(it->getSend().size())) {					 //if send failed

					it->setSendOk(true);
					it->setSend("");
				} else {															 //here is what we do if we had send all the response to the client
					it->setSend("");
					it->setSendOk(true);
																					 //at this stage the request from the client should be satisfied
				}
			}
		}
	}
}

void	SocketServer::handleClient() {
	this->simultaneousRead();
	this->simultaneousWrite();
	this->closeClean();
}

bool	SocketServer::setUpBlockServer() {
	for (size_t i = 0; i < _config.getConfig().size(); i++) {
		if (this->bindSocket(_config.getConfig().at(i)) == true)
			return true;
	}
	return false;
}

void	SocketServer::run() {
	if (this->setUpBlockServer())
		return ;
	while (true) {
		try {
			this->FdZero();
			for (size_t i = 0; i < _config.getConfig().size(); i++) {
				this->setFdSet(_config.getConfig().at(i));
			}
			this->selectSocket();
			for (size_t i = 0; i < _config.getConfig().size(); i++) {
				this->setClientSocket(_config.getConfig().at(i));
			}
			this->handleClient();
		} catch (std::exception &e) {
			std::cerr << e.what() << std::endl;
		}
	}
}

void	SocketServer::closeClean() {
	std::vector<ClientManager>::iterator		it;
	std::vector<ClientManager>::iterator		ite;

	ite = this->getClientSocket().end();

	for (it = this->getClientSocket().begin(); it != ite; it++) {				 //this loop is dedicated to close client fds when we're done with them

		if (it->getReadOk() && it->getSendOk()) {								//this is the basic case, we read all de request and send alll the response

			FD_CLR(it->getFd(), &_writefds);
			FD_CLR(it->getFd(), &_readfds);
			close(it->getFd());
			this->getClientSocket().erase(it);
		} else if (it->getReadOk() && it->getSend() == "") {					//this is the send fail case

			close(it->getFd());
			FD_CLR(it->getFd(), &_readfds);
			this->getClientSocket().erase(it);
		} else if (it->getSendOk()) {											//this the write only case (meaning tha the fd was only ready for writing)

			close(it->getFd());
			FD_CLR(it->getFd(), &_writefds);
			this->getClientSocket().erase(it);
		}
	}
}
