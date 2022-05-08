#include "Config.hpp"

Config::Config(const char *av)
	: _in_file(av), _last_instruction(""), _word(""), _block_index(-1), _loc_id(-1),
	_new_instruction(true) {
	this->parsing();
}

Config::~Config() {
}

void	Config::addConfig() {
	BlockConfig	conf;

	_block_index++;
	_loc_id = -1;
	_config.push_back(conf);
	_in_file >> _word;
}

std::vector<BlockConfig>	Config::getConfig() const {
	return _config;
}

std::string	Config::checkEndOfLine(char c) {
	std::string	tmp;
	size_t		found = 0;

	found = _word.find_first_of(c);
	tmp = _word;
	if (found == _word.size() - 1) {
//		tmp = _word.substr(0, found);
		_word = _word.substr(0, found);
//		std::cerr << "found: " << found
//			<< "size : " <<  _word.size() - 1 << '\n' << _word << std::endl;
//		if (tmp != _word)
		_new_instruction = true;
	}
	return _word;//tmp;
}

void	Config::checkSemiColon() {
	size_t		found = 0;

	found = _word.find_first_of(';');
	if (found == _word.size() - 1) {
//		std::cerr << "Semi colon found" << std::endl;
		_new_instruction = true;
	}
	else
		_new_instruction = false;
}

bool	isInstruction(const std::string &word) {
	if (word == "listen" || word == "server_name" || word == "root"
		|| word == "index" || word == "location" || word == "server"
		|| word == "{" || word == "}")
		return true;
	return false;
}

void	Config::parsPort() {
	std::cerr << "Port =" << _word << std::endl;
	this->checkEndOfLine(';');
//	_word = this->checkEndOfLine(';');
//	for (size_t i = 0; i < _word.size(); i++) {
//		if (!isdigit(_word[i]))
//			exit(EXIT_FAILURE);
//	}
	_config.at(_block_index).setNewPort(atoi(_word.c_str()));
}

void	Config::parsServerName() {
	_config.at(_block_index).setNewServerName(this->checkEndOfLine(';'));
}

void	Config::parsRoot() {
	_config.at(_block_index).setNewRoot(this->checkEndOfLine(';'));
}

void	Config::parsIndex() {
	std::string	tmp = _word;

	this->checkSemiColon();
	if (_new_instruction)
	{
		_config.at(_block_index).setNewIndex(this->checkEndOfLine(';'));
	}
	else
	{
		std::cerr << "No semi colon = " << _word << std::endl;
		_config.at(_block_index).setNewIndex(_word);
	}
}

void	Config::parsLocation(int &location_scope) {
//	std::cerr << _word << std::endl;
	if (location_scope == false) {
		_config.at(_block_index).setNewLocation(_word);
		_in_file >> _word;
		if (_word == "{") {
			std::cerr << "Open location" << std::endl;
			location_scope = true;
		} else {
			std::cerr << "aSWD" << std::endl;
			this->errorBadConf();
		}
	} else {
		if (_word == "}") {
			std::cerr << "Close location" << std::endl;
			location_scope = false;
			return ;
		}
		std::cerr << _word << std::endl;
//		_config.at(_block_index).addPathToLocation(_word, _loc_id);
		_config.at(_block_index).addPathToLocation(this->checkEndOfLine(';'), _loc_id);
	}
}

void	Config::errorBadConf() const {
	std::cerr << "Error: Bad conf format" << std::endl;
	exit(EXIT_FAILURE);
}

void	Config::errorBadKeyword() const {
	std::cerr << "Cursor on : \"" << _word << "\"" << std::endl;
	std::cerr << "Error: Bad Key _word" << std::endl;
	std::cerr << "_word = " << _word << std::endl;
	exit(EXIT_FAILURE);
}

void	Config::errorScopeDepth() const {
	std::cerr << "Cursor on : \"" << _word << "\"" << std::endl;
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
		for (size_t i = 0; i < _config.at(j).getIndex().size(); i++) {
			if (i == 0)
				std::cout << "	index = ";
			std::cout << _config.at(j).getIndex().at(i);
			if (i == _config.at(j).getIndex().size() - 1)
				std::cout << std::endl;
			else
				std::cout << " ";
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

void	Config::parsing() {
	if (_in_file) {
		bool		server_scope = false;
		int			location_scope = false;

		while (_in_file >> _word) {
			std::cerr << "?" << _word << "?" << std::endl;
			if (server_scope == false) {
				this->printAllConfig();
				if (_word == "server") {
					this->addConfig();
					if (_word == "{") {
						std::cerr << "Coucou" << std::endl;
						_new_instruction = true;
						server_scope = true;
//						continue ;
					} else {
						std::cerr << "asdfg" << std::endl;
						this->errorBadConf();
					}
				} else {
						std::cerr << "asdfg" << std::endl;
					this->errorBadConf();
				}
			} else {
				if (_word == "}" && location_scope == false) {
					std::cerr << "Coucou " << std::endl;
					server_scope = false;
					std::cerr << location_scope << std::endl;
				} else {
					if (location_scope == false) {
						if (_new_instruction == true) {
							if (!isInstruction(_word)) {
								this->errorBadConf();
							}
							_new_instruction = false;
							if (_word == "listen") {
								_last_instruction = _word;
							} else if (_word == "server_name") {
								_last_instruction = _word;
							} else if (_word == "root") {
								_last_instruction = _word;
							} else if (_word == "index") {
								_last_instruction = _word;
							}
							else if (_word == "location") {
								_loc_id++;
								_last_instruction = _word;
							}
						} else if (_new_instruction == false) {
							if (_last_instruction == "listen") {
								this->parsPort();
							} else if (_last_instruction == "server_name") {
								this->parsServerName();
							} else if (_last_instruction == "root") {
								this->parsRoot();
							} else if (_last_instruction == "index") {
								this->parsIndex();
							} else if (_last_instruction == "location") {
								this->parsLocation(location_scope);
							}
							else {
								std::cerr << "no_instru" << std::endl;
								this->errorBadConf();
							}
						}
					} else {
						std::cerr << "PAR ICI" << std::endl;
						this->parsLocation(location_scope);
					}
				}
			}
		}
		this->printAllConfig();
	} else {
		std::cerr << "Can't read conf file" << std::endl;
	}
}
