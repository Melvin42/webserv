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

		// first passage to extract file and check end of lines
		while (std::getline(in_file, line)) {
			line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
			if (line.compare("") != 0) {
				if (line[line.size() - 1] == ';'
							|| line[line.size() - 1] == '{'
							|| line[line.size() - 1] == '}')
					conf.push_back(line);
				else {
					std::cerr << "Error: Bad End of line" << std::endl;
					exit(EXIT_FAILURE);
				}
			}
		}

		std::vector<std::string>::iterator			it = conf.begin();
		std::vector<std::string>::const_iterator	ite = conf.end();

		std::string	search;

//		std::cerr << *it << std::endl;

//		std::vector<std::string>::iterator			tmp;
		std::string	tmp;
		size_t	found = 0;
		it = conf.begin();
		ite = conf.end();
		while (it != ite) {
//			search = std::find(it.begin(), conf.end(), "listen");
//		std::cout << "search:" << search << std::endl;
			search = *it;
			found = search.find("listen");
			if (found != std::string::npos) {
				std::cerr << "found = " <<  found << std::endl;
				std::cerr << search[found + 6] << std::endl;
				if (search.find(';') != std::string::npos && search[search.size() - 1] == ';')
					std::cout << ";;;;;;;; OK!" << std::endl;

				tmp = search.erase(0, 6);
				int	nbr = 0;

				nbr = atoi(tmp.c_str());
				tmp = std::to_string(nbr); //atoi this end this is good!
				std::cerr << "tmp = " << tmp << std::endl;
				std::cerr << "port = " << _port << std::endl;

				_config.insert(std::pair<std::string, std::string>("listen", tmp.c_str()));

//				std::map<std::string, std::string>::const_iterator	tmp;
				_port = atoi(_config.find("listen")->second.c_str());
				std::cerr << "port = " << _port << std::endl;
//				_port = 8080;
			}
			std::cerr << *it << std::endl;
			it++;
		}
	} else {
		std::cerr << "Can't read conf file" << std::endl;
	}
}
