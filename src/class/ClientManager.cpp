#include "ClientManager.hpp"

/**** CONDESTRUCT ****/

ClientManager::ClientManager() : _fd(1), _header_ok(false), 
	_read_ok(false), _send_ok(false), _read(""), _send("") {
}

ClientManager::ClientManager(int fd) : _fd(fd), _header_ok(false), _read_ok(false),
	_send_ok(false), _read(""), _send("") {
}

ClientManager::~ClientManager() {
}

/**** SETGET ****/

int	ClientManager::getFd() const {
	return _fd;
}

bool	ClientManager::getHeaderOk() const {
	return _header_ok;
}

bool	ClientManager::getSendOk() const {
	return _send_ok;
}

bool	ClientManager::getReadOk() const {
	return _read_ok;
}

std::string	ClientManager::getRead() const {
	return _read;
}

std::string	ClientManager::getSend() const {
	return _send;
}

void	ClientManager::setFd(int fd) {
	_fd = fd;
}

void	ClientManager::setHeaderOk(bool ok) {
	_header_ok = ok;
}

void	ClientManager::setSendOk(bool ok) {
	_send_ok = ok;
}

void	ClientManager::setReadOk(bool ok) {
	_read_ok = ok;
}

void	ClientManager::setRead(std::string str) {
	_read = str;
}

void	ClientManager::setSend(std::string str) {
	_send = str;
}

/**** USE ****/

void	ClientManager::appendRead(char *buf) {
	_read.append(buf);
}

bool	ClientManager::isReadOk() {
	HttpRequest	req(_read.c_str(), static_cast<int>(_read.size()));
	if (req.getBody().size() == req.getContentLength()) {
		_read_ok = true;
	}
	return _read_ok;
}
