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

std::vector<std::string>	BlockConfig::getServerName() const {
	return _server_name;
}

std::vector<std::string>	BlockConfig::getLocation() const {
	return _location;
}

void	BlockConfig::setNewPort(int port) {
	_port.push_back(port);
}

void	BlockConfig::setNewServerName(std::string name) {
	_server_name.push_back(name);
}

void	BlockConfig::setNewRoot(std::string root) {
	_root.push_back(root);
}

void	BlockConfig::setNewLocation(std::string loc) {
	_location.push_back(loc);
}
