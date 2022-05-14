#include "BlockConfig.hpp"

BlockConfig::BlockConfig() : _default_index(""), _default_404(""),
	_port(8080), _server_name("localhost"), _root("./") {//, _can_post(false), _autoindex(false), _body_size_max(0) {
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

void	BlockConfig::setDefaultIndex() {
	for (size_t i = 0; i < _index.size(); i++) {
		std::string	tmp = _root + "/" + _index.at(i);
		if (access(tmp.c_str(), R_OK) == 0) {
			_default_index = tmp;
			return ;
		}
	}
	for (size_t i = 0; i < this->getLocation().size(); i++) {
		for (size_t j = 0; j < this->getLocation().at(i).getIndex().size(); j++) {
			std::string	tmp = _root + "/" + this->getLocation().at(i).getArg() + "/" + this->getLocation().at(i).getIndex().at(j);
			if (access(tmp.c_str(), R_OK) == 0) {
				_default_index = tmp;
				return ;
			}
		}
	}
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

void	BlockConfig::addCgiToLocationMap(const std::string &key, const std::string &value, int loc_id) {
	this->_location.at(loc_id).addCgiPath(key, value);
}
