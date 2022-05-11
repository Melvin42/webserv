#include "Location.hpp"

Location::Location() : _arg(""), _type(""),
	_cgi_binary_nbr(0), _cgi_filename_nbr(0) {
}

Location::Location(const std::string &arg) : _arg(arg), _type(""),
	_cgi_binary_nbr(0), _cgi_filename_nbr(0) {
}

Location::~Location() {
}

std::string	Location::getArg() const {
	return _arg;
}

std::vector<std::string>	Location::getIndex() const {
	return _index;
}

std::vector<std::string>	Location::getCgiBinary() const {
	return _cgi_binary;
}

std::vector<std::string>	Location::getCgiFilename() const {
	return _cgi_filename;
}

int	Location::getCgiBinaryNbr() const {
	return _cgi_binary_nbr;
}

int	Location::getCgiFilenameNbr() const {
	return _cgi_filename_nbr;
}

void	Location::addIndex(const std::string &index) {
	_index.push_back(index);
}

void	Location::addCgiBinary(const std::string &cgi_binary) {
	_cgi_binary.push_back(cgi_binary);
}

void	Location::addCgiFilename(const std::string &cgi_filename) {
	_cgi_filename.push_back(cgi_filename);
}

void	Location::incCgiBinaryNbr() {
	_cgi_binary_nbr++;
}

void	Location::incCgiFilenameNbr() {
	_cgi_filename_nbr++;
}
