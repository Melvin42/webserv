#include "Location.hpp"

Location::Location() : _arg(""), _root(""), _type("") {
}

Location::Location(const std::string &arg) : _arg(arg), _root(""), _type("") {
}

Location::Location(const Location &cp) {
	*this = cp;
}

Location::~Location() {
}

std::string	Location::getArg() const {
	return _arg;
}

std::string	Location::getRoot() const {
	return _root;
}

std::string	Location::getType() const {
	return _type;
}

std::vector<std::string>	Location::getIndex() const {
	return _index;
}

std::map<std::string, std::string>	Location::getCgiMap() const {
	return _cgi;
}

void	Location::setRoot(const std::string &root) {
	if (_root == "")
		_root = root;
}

void	Location::setType(const std::string &type) {
	_type = type;
}

void	Location::addIndex(const std::string &index) {
	_index.push_back(index);
}

void	Location::printCgiMap() {
	std::map<std::string, std::string>::const_iterator	it = _cgi.begin();

	for(it = _cgi.begin(); it != _cgi.end(); it++) {
		std::cerr << "		cgi-bin BINARY = " << it->second << "\n"
			<< "		cgi-bin SCRIP_EXT = " << it->first << std::endl;
	}
}

void	Location::addCgiPath(const std::string &key, const std::string &value) {
	_cgi.insert(std::pair<std::string, std::string>(key, value));
}

Location &Location::operator=(const Location &loc) {
	_arg = loc._arg;
	_root = loc._root;
	_type = loc._type;
	_index = loc._index;
	_cgi = loc._cgi;
	return *this;
}
