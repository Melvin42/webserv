#include "Config.hpp"

Config::Config() {
}

Config::Config(const char *av) {
	this->parsing(av);
}
Config::Config(const Config &config) {
	*this = config;
}

Config::~Config() {
}

std::map<std::string, std::string> Config::getConfig() const {
	return _config;
}

void	Config::printMap() const {
	std::map<std::string, std::string>::const_iterator	it = _config.begin();
	for(it = _config.begin(); it != _config.end(); it++) {
		std::cout << '{' << (*it).first << ':' << (*it).second << "}\n"; }
}

void	Config::parsing(const char *av) {
	std::ifstream	in_file(av);

	if (in_file) {
		std::vector<std::string>	conf;
		std::string					line;

		while (std::getline(in_file, line)) {
			conf.push_back(line);
		}

		std::vector<std::string>::iterator			it = conf.begin();
		std::vector<std::string>::const_iterator	ite = conf.end();

		while (it != ite) {
			std::cerr << *it << std::endl;
			it++;
		}
	} else {
		std::cerr << "Can't read conf file" << std::endl;
	}
}
