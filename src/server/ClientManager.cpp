#include "ClientManager.hpp"

/**** CONDESTRUCT ****/

ClientManager::ClientManager() : _fd(0), _header_ok(false), 
	_read_ok(false), _send_ok(false), _read(""), _send("") {
}

ClientManager::ClientManager(int fd, const BlockConfig &block)
	: _fd(fd), _header_ok(false), _read_ok(false),
	_send_ok(false), _read(""), _send(""), _block(block) {
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

BlockConfig	ClientManager::getBlock() const {
	return _block;
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
//	_read.append(buf);
	_read += buf;
}

bool	ClientManager::isReadOk() {
	std::stringstream 	buf;
	std::string			line;
	std::string			boundry;
	std::string			boundryEnd;
	size_t				bodySize = 0;
	size_t				contentLength = 0;

//	if (_read != "") {
		buf <<  _read;
		while (std::getline(buf, line))
		{
			if (buf.eof() || buf.bad() || line == "\r")
				break ;
			if (line.find("Content-Length: ") != std::string::npos)
				contentLength = std::atoi(line.substr(line.find("Content-Length: ") + 16).c_str());
			if (line.find("boundary=") != std::string::npos)
			{
				boundry = "--" + line.substr(line.find("boundary=") + 9) + "\r";
				boundryEnd = "--" + line.substr(line.find("boundary=") + 9) + "--" + "\r";
			}
		}
		if (contentLength)
		{
			while (std::getline(buf, line))
			{
				if (buf.eof() || buf.bad())
					break ;
				bodySize += line.size() + 1;
				if (line == boundryEnd)
					break ;
			}
		}
		if (contentLength == bodySize)
			_read_ok = true;
//	}
	return _read_ok;
}
