#include "ClientManager.hpp"

/**** CONDESTRUCT ****/

ClientManager::ClientManager() : _fd(1), _header_ok(false), 
	_read_ok(0), _send_ok(false), _read(""), _send(""), _valRead(0) {
}

ClientManager::ClientManager(int fd, const BlockConfig &block) : _fd(fd), _header_ok(false), _read_ok(false),
	_send_ok(false), _read(""), _send(""), _valRead(0), _block(block) {
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

int		ClientManager::getReadOk() const {
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

void	ClientManager::setReadOk(int ok) {
	_read_ok = ok;
}

void	ClientManager::setRead(std::string str) {
	_read = str;
}

void	ClientManager::setSend(std::string str) {
	_send = str;
}

/**** USE ****/

void		ClientManager::incrementValRead(int valread) {
	_valRead += valread;
}

void	ClientManager::appendRead(char *buf, int valread) {
	_read.append(buf, valread);
	// std::cout << "append: " << valread << "cur size: " << _read.size() << std::endl;
}

int		ClientManager::isReadOk() {
	std::stringstream 	buf;
	std::string			line;
	std::string			boundry;
	std::string			boundryEnd;
	size_t				headerSize = 0;
	size_t				bodySize = 0;
	size_t				contentLength = 0;

	buf << _read;
	while (std::getline(buf, line))
	{
		if (buf.eof() || buf.bad() || line == "\r")
			break ;
		if (line.find("content-length: ") != std::string::npos)
			contentLength = std::atoi(line.substr(line.find("content-length: ") + 16).c_str());
		if (line.find("boundary=") != std::string::npos)
		{
			boundry = "--" + line.substr(line.find("boundary=") + 9) + "\r";
			boundryEnd = "--" + line.substr(line.find("boundary=") + 9) + "--" + "\r";
		}
		headerSize += line.size() + 1;
	}
	headerSize += 2;
	line = buf.str();
	if (contentLength)
		bodySize = _valRead - headerSize;
	if (contentLength == bodySize)
	{	_read_ok = 1;
	// std::cerr << "contentLength: " << contentLength << "...\n";
	}return _read_ok;
}
