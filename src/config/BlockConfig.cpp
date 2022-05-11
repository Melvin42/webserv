#include "BlockConfig.hpp"

BlockConfig::BlockConfig() : _port(8080), _server_name("localhost"), _root("./") {
}

BlockConfig::~BlockConfig() {
}

int	BlockConfig::getPort() const {
	return _port;
}

std::string	BlockConfig::getRoot() const {
	return _root;
}

std::vector<std::string>	BlockConfig::getIndex() const {
	return _index;
}

std::string	BlockConfig::getServerName() const {
	return _server_name;
}

std::vector<Location>	BlockConfig::getLocation() const {
	return _location;
}

void	BlockConfig::setNewPort(int port) {
	_port = port;
}

void	BlockConfig::setNewServerName(const std::string &name) {
	_server_name = name;
}

void	BlockConfig::setNewRoot(const std::string &root) {
	_root = root;
}

void	BlockConfig::setNewIndex(const std::string &index) {
	_index.push_back(index);
}

void	BlockConfig::setNewLocation(const std::string &arg) {
	Location	loc(arg);

	_location.push_back(loc);
}

void	BlockConfig::addIndexToLocation(const std::string &index, int i) {
	this->_location.at(i).addIndex(index);
}

void	BlockConfig::addCgiBinaryToLocation(const std::string &binary, int i) {
	this->_location.at(i).addCgiBinary(binary);
}

void	BlockConfig::addCgiFilenameToLocation(const std::string &filename, int i) {
	this->_location.at(i).addCgiFilename(filename);
}

void	BlockConfig::incCgiBinaryNbrLocation(int i) {
	this->_location.at(i).incCgiBinaryNbr();
}

void	BlockConfig::incCgiFilenameNbrLocation(int i) {
	this->_location.at(i).incCgiFilenameNbr();
}
