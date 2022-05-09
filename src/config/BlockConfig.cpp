#include "BlockConfig.hpp"

BlockConfig::BlockConfig() {
}

BlockConfig::~BlockConfig() {
}

std::vector<int>	BlockConfig::getPort() const {
	return _port;
}

std::vector<std::string>	BlockConfig::getRoot() const {
	return _root;
}

std::vector<std::string>	BlockConfig::getIndex() const {
	return _index;
}

std::vector<std::string>	BlockConfig::getServerName() const {
	return _server_name;
}

std::vector<Location>	BlockConfig::getLocation() const {
	return _location;
}

void	BlockConfig::setNewPort(int port) {
	_port.push_back(port);
}

void	BlockConfig::setNewServerName(const std::string &name) {
	_server_name.push_back(name);
}

void	BlockConfig::setNewRoot(const std::string &root) {
	_root.push_back(root);
}

void	BlockConfig::setNewIndex(const std::string &index) {
	_index.push_back(index);
}

void	BlockConfig::setNewLocation(const std::string &arg) {
	Location	loc(arg);

	_location.push_back(loc);
}
void	BlockConfig::addPathToLocation(const std::string &path, int index) {
	this->_location.at(index).addPath(path);
}
