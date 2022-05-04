#include "Config.hpp"

Config::Config(const char *av) {
	this->parsing(av);
}

Config::~Config() {
}

void	Config::addConfig() {
	BlockConfig	conf;

	_config.push_back(conf);
}

std::vector<BlockConfig>	Config::getConfig() const {
	return _config;
}

std::string	Config::checkEndOfLine(const std::string &word, char c) {
	std::string	tmp;
	size_t		found = 0;

	found = word.find_first_of(c);
	if (found != word.size() - 1) {
		std::cerr << "Error: line isn't finish by " << c << std::endl;
		exit(EXIT_FAILURE);
	}
	tmp = word.substr(0, found);
	return tmp;
}

void	Config::parsPort(const std::string &word, int block_index) {
	for (size_t i = 0; i < word.size(); i++) {
		if (!isdigit(word[i]))
			exit(EXIT_FAILURE);
	}
	_config.at(block_index).setNewPort(atoi(word.c_str()));
}

void	Config::parsServerName(const std::string &word, int block_index) {
	_config.at(block_index).setNewServerName(word.c_str());
}

void	Config::parsRoot(const std::string &word, int block_index) {
	_config.at(block_index).setNewRoot(word);
}

// Need to change type string for location to get more infos
void	Config::parsLocation(const std::string &word, int block_index) {
	_config.at(block_index).setNewLocation(word);
}

void	Config::printAllConfig() const {
	for (size_t j = 0; j < _config.size(); j++) {
		std::cout << "\nBlock Nb : " << j+1 << '\n' << std::endl;
		for (size_t i = 0; i < _config.at(j).getPort().size(); i++) {
			std::cout << "	port = " << _config.at(j).getPort().at(i) << std::endl;
		}
		for (size_t i = 0; i < _config.at(j).getServerName().size(); i++) {
			std::cout << "	server_name = " << _config.at(j).getServerName().at(i) << std::endl;
		}
		for (size_t i = 0; i < _config.at(j).getRoot().size(); i++) {
			std::cout << "	root = " << _config.at(j).getRoot().at(i) << std::endl;
		}
		for (size_t i = 0; i < _config.at(j).getLocation().size(); i++) {
			std::cout << "	location = " << _config.at(j).getLocation().at(i) << std::endl;
		}
	}
}

void	Config::parsing(const char *av) {
	std::ifstream	in_file(av);

	if (in_file) {
		std::string	word;
		bool		server_scope = false;
		bool		location_scope = false;
		int			block_index = -1;


		while (in_file >> word) {
			if (server_scope == false) {
				if (word == "server") {
					block_index++;
					this->addConfig();
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
				if (word == "}") {
					server_scope = false;
					continue ;
				}
				if (location_scope == false) {
					if (word == "listen") {
						in_file >> word;
						this->parsPort(this->checkEndOfLine(word, ';'), block_index);
					} else if (word == "server_name") {
						in_file >> word;
						this->parsServerName(this->checkEndOfLine(word, ';'), block_index);
					} else if (word == "root") {
						in_file >> word;
						this->parsRoot(this->checkEndOfLine(word, ';'), block_index);
					} else if (word == "location") {
						in_file >> word;
						in_file >> word;
						this->checkEndOfLine(word, '{');
						while (word != "{") {
							in_file >> word;
						}
						if (word == "{") {
							in_file >> word;
							location_scope = true;
						}
						if (word == "index")
							in_file >> word;
						this->parsLocation(this->checkEndOfLine(word, ';'), block_index);
						in_file >> word;
						if (word == "}") {
							location_scope = false;
//							in_file >> word;
						}
					} else {
						std::cerr << "Cursor on : \"" << word << "\"" << std::endl;
						std::cerr << "Error: Bad Key word" << std::endl;
						std::cerr << "word = " << word << std::endl;
						exit(EXIT_FAILURE);
					}
				} else if (word == "{") {
						std::cerr << "Cursor on : \"" << word << "\"" << std::endl;
						std::cerr << "Error: Trying to go depth 3 scope" << std::endl;
						exit(EXIT_FAILURE);
				}
			}
		}
		this->printAllConfig();
	} else {
		std::cerr << "Can't read conf file" << std::endl;
	}
}
