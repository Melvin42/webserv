#include "BlockConfig.hpp"

BlockConfig::BlockConfig() : _default_index(""), _default_404(""),
	_host("0.0.0.0"), _port(8080), _server_name("localhost"), _root("./"),
	_can_post(true), _can_get(true), _can_delete(true),
	_autoindex(false), _body_size_max(0) {
}

BlockConfig::~BlockConfig() {
}

std::string	BlockConfig::getHost() const {
	return _host;
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

std::string	BlockConfig::getDefaultIndex() const {
	return _default_index;
}

std::string	BlockConfig::getDefault404() const {
	return _default_404;
}

bool	BlockConfig::getCanPost() const {
	return _can_post;
}

bool	BlockConfig::getCanGet() const {
	return _can_get;
}

bool	BlockConfig::getCanDelete() const {
	return _can_delete;
}

bool	BlockConfig::getAutoindex() const {
	return _autoindex;
}

size_t	BlockConfig::getBodySizeMax() const {
	return _body_size_max;
}

size_t	BlockConfig::getId() const {
	return _id;
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

void	BlockConfig::setDefault404() {
	(void)_default_404;
}

void	BlockConfig::setNewHost(const std::string &host) {
	_host = host;
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

void	BlockConfig::setCanPost(const bool &can_post) {
	_can_post = can_post;
}

void	BlockConfig::setCanGet(const bool &can_get) {
	_can_get = can_get;
}

void	BlockConfig::setCanDelete(const bool &can_delete) {
	_can_delete = can_delete;
}

void	BlockConfig::setAutoindex(const bool &autoindex) {
	_autoindex = autoindex;
}

void	BlockConfig::setBodySizeMax(const size_t &size_max) {
	_body_size_max = size_max;
}

void	BlockConfig::setId(const size_t &id) {
	_id = id;
}
