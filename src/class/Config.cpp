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

int	Config::getPort() const {
	return _port;
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
			line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
//			std::cout << "line:" << line << std::endl;
			if (line.compare("") != 0)
				conf.push_back(line);
		}

		std::vector<std::string>::iterator			it = conf.begin();
		std::vector<std::string>::const_iterator	ite = conf.end();

		std::string	search = *conf.begin();

		std::cout << search.find_last_of('{') << std::endl;
		std::cout << search.length() << std::endl;
		if (search.find_last_of('{') == search.length() - 1)
			std::cout << "OK!" << std::endl;

//		it = search.find(search.begin(), search.end(), "server");

		std::cerr << *it << std::endl;
//		_server_name = *it;

//		std::cout << _server_name << std::endl;

//		std::cout << search[0] << std::endl;

		it = conf.begin();
		ite = conf.end();
		while (it != ite) {
//			search = std::find(it.begin(), conf.end(), "listen");
		std::cout << "search:" << search << std::endl;
			std::cerr << *it << std::endl;
			it++;
		}
		_port = 8080;
	} else {
		std::cerr << "Can't read conf file" << std::endl;
	}
}
