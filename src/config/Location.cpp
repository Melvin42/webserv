#include "Location.hpp"

Location::Location() : _arg(""), _type("") {
}

Location::Location(const std::string &arg) : _arg(arg), _type("") {
}

Location::~Location() {
}

std::string	Location::getArg() const {
	return _arg;
}

std::vector<std::string>	Location::getIndex() const {
	return _index;
}

std::map<std::string, std::string>	Location::getCgiMap() const {
	return _cgi;
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
