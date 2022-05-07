#include "Config.hpp"

Config::Config(const char *av) {
	this->parsing(av);
}

Config::~Config() {
}

std::string	Config::addConfig(std::ifstream &stream, int &block_index, int &loc_id) {
	std::string	word;
	BlockConfig	conf;

	block_index++;
	loc_id = -1;
	_config.push_back(conf);
	stream >> word;
	return word;
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

void	Config::parsPort(std::ifstream &stream, int block_index) {
	std::string	word;

	stream >> word;
	word = this->checkEndOfLine(word, ';');
	for (size_t i = 0; i < word.size(); i++) {
		if (!isdigit(word[i]))
			exit(EXIT_FAILURE);
	}
	_config.at(block_index).setNewPort(atoi(word.c_str()));
}

void	Config::parsServerName(std::ifstream &stream, int block_index) {
	std::string	word;

	stream >> word;
	_config.at(block_index).setNewServerName(this->checkEndOfLine(word, ';').c_str());
}

void	Config::parsRoot(std::ifstream &stream, int block_index) {
	std::string	word;

	stream >> word;
	_config.at(block_index).setNewRoot(this->checkEndOfLine(word, ';'));
}

void	Config::parsLocation(std::ifstream &stream, int block_index,
		int loc_id, int &location_scope) {
	std::string	word;

	stream >> word;
	_config.at(block_index).setNewLocation(word);
	stream >> word;
	this->checkEndOfLine(word, '{');
	std::cerr << word << std::endl;
	if (word == "{") {
		stream >> word;
		location_scope = true;
	}
	while (word != "}") {
		_config.at(block_index).addPathToLocation(word, loc_id);
		stream >> word;
	}
	if (word == "}")
		location_scope = false;
}

void	Config::errorBadConf() const {
	std::cerr << "Error: Bad conf format" << std::endl;
	exit(EXIT_FAILURE);
}

void	Config::errorBadKeyword(const std::string &word) const {
	std::cerr << "Cursor on : \"" << word << "\"" << std::endl;
	std::cerr << "Error: Bad Key word" << std::endl;
	std::cerr << "word = " << word << std::endl;
	exit(EXIT_FAILURE);
}

void	Config::errorScopeDepth(const std::string &word) const {
	std::cerr << "Cursor on : \"" << word << "\"" << std::endl;
	std::cerr << "Error: Trying to go depth 3 scope" << std::endl;
	exit(EXIT_FAILURE);
}

void	Config::printAllConfig() const {
	for (size_t j = 0; j < _config.size(); j++) {
		std::cout << "\nBlock Nb : " << j+1 << '\n' << std::endl;
		for (size_t i = 0; i < _config.at(j).getPort().size(); i++) {
			std::cout << "	port = "
				<< _config.at(j).getPort().at(i) << std::endl;
		}
		for (size_t i = 0; i < _config.at(j).getServerName().size(); i++) {
			std::cout << "	server_name = "
				<< _config.at(j).getServerName().at(i) << std::endl;
		}
		for (size_t i = 0; i < _config.at(j).getRoot().size(); i++) {
			std::cout << "	root = "
				<< _config.at(j).getRoot().at(i) << std::endl;
		}
		for (size_t i = 0; i < _config.at(j).getLocation().size(); i++) {
			std::cout << "	location = "
				<< _config.at(j).getLocation().at(i).getArg() << std::endl;
			for (size_t k = 0; k < _config.at(j).getLocation().at(i).getPath().size(); k++) {
				std::cout << "		path = "
					<< _config.at(j).getLocation().at(i).getPath().at(k)
					<< std::endl;
			}
		}
	}
}

void	Config::parsing(const char *av) {
	std::ifstream	in_file(av);

	if (in_file) {
		std::string	word;
		bool		server_scope = false;
		int			location_scope = false;
		int			loc_id = -1;
		int			block_index = -1;

		while (in_file >> word) {
			if (server_scope == false) {
				if (word == "server") {
					word = this->addConfig(in_file, block_index, loc_id);
					if (word == "{") {
						server_scope = true;
						continue ;
					} else
						this->errorBadConf();
				} else
					this->errorBadConf();
			} else {
				if (word == "}") {
					server_scope = false;
					continue ;
				}
				if (location_scope == false) {
					if (word == "listen")
						this->parsPort(in_file, block_index);
					else if (word == "server_name")
						this->parsServerName(in_file, block_index);
					else if (word == "root")
						this->parsRoot(in_file, block_index);
					else if (word == "location") {
						loc_id++;
						this->parsLocation(in_file, block_index, loc_id, location_scope);
					} else
						this->errorBadKeyword(word);
				} else if (word == "{")
						this->errorScopeDepth(word);
			}
		}
		this->printAllConfig();
	} else {
		std::cerr << "Can't read conf file" << std::endl;
	}
}
