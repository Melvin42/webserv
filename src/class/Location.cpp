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

std::vector<std::string>	Location::getPath() const {
	return _path;
}

void	Location::addPath(const std::string &path) {
	_path.push_back(path);
}
