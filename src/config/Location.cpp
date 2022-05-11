#include "Location.hpp"

Location::Location() : _arg("") {
}

Location::Location(const std::string &arg) : _arg(arg) {
}

Location::~Location() {
}

std::string	Location::getArg() const {
	return _arg;
}

std::vector<std::string>	Location::getIndex() const {
	return _index;
}

void	Location::addIndex(const std::string &index) {
	_index.push_back(index);
}

std::vector<std::string>	Location::getCgiBinary() const {
	return _cgi_binary;
}

void	Location::addCgiBinary(const std::string &cgi_binary) {
	_cgi_binary.push_back(cgi_binary);
}

std::vector<std::string>	Location::getCgiFilename() const {
	return _cgi_filename;
}

void	Location::addCgiFilename(const std::string &cgi_filename) {
	_cgi_filename.push_back(cgi_filename);
}
