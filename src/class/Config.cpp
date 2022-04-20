#include "Config.hpp"

BlockConfig::BlockConfig() {
}

BlockConfig::BlockConfig(const char *av) : _port(0) {
	this->parsing(av);
}

BlockConfig::BlockConfig(const BlockConfig &config) {
	*this = config;
}

BlockConfig::~BlockConfig() {
}

std::vector<int>		BlockConfig::getPort() const {
	return _port;
}

//void	BlockConfig::printMap() const {
//	std::map<std::string, std::string>::const_iterator	it = _block.begin();
//	for(it = _config.begin(); it != _config.end(); it++) {
//		std::cout << '{' << (*it).first << ':' << (*it).second << "}\n"; }
//}

std::string	BlockConfig::checkEndOfLine(std::string word) {
	std::string	tmp;
	size_t		found = 0;

	found = word.find_first_of(';');
	if (found != word.size() - 1) {
		std::cerr << "Error: line isn't finish by ';'" << std::endl;
		exit(EXIT_FAILURE);
	}
	tmp = word.substr(0, found);
	return tmp;
}

void	BlockConfig::parsing(const char *av) {
	std::ifstream	in_file(av);

	if (in_file) {
		std::string					word;
		std::string					tmp;

//		line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
		bool		server_scope = false;
		bool		location_scope = false;
//		size_t		found = 0;

		while (in_file >> word) {
			std::cout << word << std::endl;
			if (server_scope == false) {
				if (word == "server") {
					in_file >> word;
					if (word == "{") {
						server_scope = true;
						continue ;
					} else {
						std::cerr << "Error: Bad conf format" << std::endl;
						exit(EXIT_FAILURE);
					}
				} else {
					std::cerr << "Error: Bad conf format" << std::endl;
					exit(EXIT_FAILURE);
				}
			} else {
				if (location_scope == false) {
					if (word == "listen") {
						in_file >> word;
						tmp = this->checkEndOfLine(word);
						for (size_t i = 0; i < tmp.size(); i++) {
							if (!isdigit(tmp[i])) {
								exit(EXIT_FAILURE);
							}
						}
						_port.push_back(atoi(tmp.c_str()));
						std::cout << "port = " << word << std::endl;
					} else if (word == "server_name") {
						in_file >> word;
						tmp = this->checkEndOfLine(word);
							_server_name.push_back(word.c_str());
							std::cout << "server_name = " << word << std::endl;
					} else if (word == "root") {
						in_file >> word;
						tmp = this->checkEndOfLine(word);
							_root.push_back(word);
							std::cout << "root = " << word << std::endl;
					} else if (word == "}") {
						break;
//					} else if (word == "location") {
//						in_file >> word;
//						location_scope = true;
					} else {
						std::cerr << "Error: Bad Key word" << std::endl;
						exit(EXIT_FAILURE);
					}
				}
//				if (location_scope == true) {
//					if (word == "{") {
//						std::cerr << "Error: Can't find server scope" << std::endl;
//						exit(EXIT_FAILURE);
//					}
//				}
			}
		}
	} else {
		std::cerr << "Can't read conf file" << std::endl;
	}
}
