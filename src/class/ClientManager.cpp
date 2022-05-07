#include "ClientManager.hpp"

/**** CONDESTRUCT ****/

ClientManager::ClientManager() : _fd(1), _read_ok(false),
	_send_ok(false), _read(""), _send("") {
}

ClientManager::ClientManager(int fd) : _fd(fd), _read_ok(false),
	_send_ok(false), _read(""), _send("") {
}

ClientManager::~ClientManager() {
}

/**** SETGET ****/

int	Socket::getFd() {
	return _fd;
}

bool	Socket::getSendOk() {
	return _send_ok;
}

bool	Socket::getReadOk() {
	return _read_ok;
}

std::string	Socket::getRead() {
	return _read;
}

std::string	Socket::getSend() {
	return _send;
}

void	Socket::setFd(int fd) {
	_fd = fd;
}

void	Socket::setSendOk(bool ok) {
	_send_ok = ok;
}

void	Socket::setReadOk(bool ok) {
	_read_ok = ok;
}

void	Socket::setRead(std::string str) {
	_read = str;
}

void	Socket::setSend(std::string str) {
	_send = str;
}

/**** USE ****/

void	Socket::appendRead(char *buf) {
	_read.append(buf);
}

bool	ClientManager::isReadOk() {
	HttpRequest	req(_read, _read.size());
	if (req.getBody().size() == req.getContentLength()) {
		_read_ok = true;
	}
	return _read_ok;
}
