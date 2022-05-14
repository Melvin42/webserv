#include "Config.hpp"

Config::Config()
	: _need_exit(false), _check_binary("SCRIPT_EXT"), _last_instruction(""), _word(""), _path(""),
	_block_index(-1), _loc_id(-1), _new_instruction(true) {
}

Config::Config(Config &cp) {
	*this = cp;
}

Config::Config(const char *av)
	: _need_exit(false), _check_binary("SCRIPT_EXT"), _in_file(av), _last_instruction(""), _word(""), _path(""),
	_block_index(-1), _loc_id(-1), _new_instruction(true) {
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

std::string	Config::getPath() const {
	return _path;
}

bool	Config::getNeedExit() const {
	return _need_exit;
}

void	Config::setNeedExit(const bool &need_exit) {
	_need_exit = need_exit;
}

void	Config::setLastInstruction(const std::string &instru) {
	_last_instruction = instru;
}

void	Config::setWord(const std::string &word) {
	_word = word;
}

void	Config::setPath(const std::string &path) {
	_path = path;
}

void	Config::concatPath() {
	if (_config.at(0).getRoot() != "")
		_path = _config.at(0).getRoot();
}

void	Config::setAllDefaultValue() {
	for (size_t i = 0; i < _config.size(); i++) {
		_config.at(i).setDefaultIndex();
	}

}

void	Config::setBlockIndex(const int &index) {
	_block_index = index;
}

void	Config::setLocId(const int &id) {
	_loc_id = id;
}

void	Config::setBlockInstruction(const int &flag) {
	_new_instruction = flag;
}

void	Config::setConfig(const std::vector<BlockConfig> &conf) {
	_config = conf;
}

std::string	Config::badEndOfLine() {
	size_t		found = 0;

	found = _word.find_first_of(';');
	if (found == _word.size() - 1) {
		_word = _word.substr(0, found);
		_new_instruction = true;
	} else
		this->errorNoSemiColon();
	return _word;
}

std::string	Config::checkEndOfLine(char c) {
	size_t		found = 0;

	found = _word.find_first_of(c);
	if (found == _word.size() - 1) {
		_word = _word.substr(0, found);
		_new_instruction = true;
	}
	return _word;
}

void	Config::checkSemiColon() {
	size_t		found = 0;

	found = _word.find_first_of(';');
	if (found == _word.size() - 1)
		_new_instruction = true;
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
	_word = this->badEndOfLine();
	for (size_t i = 0; i < _word.size(); i++) {
		if (!isdigit(_word[i]))
			this->errorBadConf();
	}
	_config.at(_block_index).setNewPort(atoi(this->_word.c_str()));
}

void	Config::parsServerName() {
	_config.at(_block_index).setNewServerName(this->badEndOfLine());
}

void	Config::parsRoot() {
	_config.at(_block_index).setNewRoot(this->badEndOfLine());
}

void	Config::parsIndex() {
	this->checkSemiColon();
	if (_new_instruction)
		_config.at(_block_index).setNewIndex(this->checkEndOfLine(';'));
	else
		_config.at(_block_index).setNewIndex(_word);
}

void	Config::parsLocationIndex() {
	std::string	tmp = _word;

	this->checkSemiColon();
	if (_new_instruction)
		_config.at(_block_index).addIndexToLocation(this->checkEndOfLine(';'), _loc_id);
	else
		_config.at(_block_index).addIndexToLocation(_word, _loc_id);
}

void	Config::parsCgi() {
	std::string valuemap;

	_in_file >> _word;
	if (_word == "BINARY" && _check_binary == "SCRIPT_EXT") {
		_in_file >> _word;
		valuemap = this->badEndOfLine();
		_check_binary = "BINARY";
		_in_file >> _word;
	}
	if (_word == "cgi-bin")
		_in_file >> _word;
	if (_word == "SCRIPT_EXT" && _check_binary == "BINARY") {
		_in_file >> _word;
		_check_binary = "SCRIPT_EXT";
		_config.at(_block_index).addCgiToLocationMap(_word, valuemap, _loc_id);
	} else {
		this->errorBadCgi();
	}
}

void	Config::parsLocation(int &location_scope) {
	if (location_scope == false) {
		_config.at(_block_index).setNewLocation(_word);
		_in_file >> _word;
		if (_word == "{") {
			location_scope = true;
			_new_instruction = false;
		} else {
			this->errorBadConf();
		}
	} else {
		if (_word == "}") {
			location_scope = false;
			_new_instruction = true;
			return ;
		}
		_new_instruction = false;
		if (_word == "index" || _word == "cgi-bin" || _word == "}") {
			if (_word == "index") {
				while (!_new_instruction && !_in_file.eof()) {
					_in_file >> _word;
					if (isInstruction(_word)) {
						this->errorBadConf();
						_new_instruction = true;
					}
					this->parsLocationIndex();
				}
			} else if (_word == "cgi-bin") {
				this->parsCgi();
			}
		} else {
			this->errorBadConf();
		}
	}
}

void	Config::errorNoSemiColon() {
	if (_need_exit == false)
		std::cerr << "Error: Missing Semi Colon" << std::endl;
	_need_exit = true;
}

void	Config::errorCantReadFile() {
	if (_need_exit == false)
		std::cerr << "Error: Can't read conf file." << std::endl;
	_need_exit = true;
}

void	Config::errorBadConf() {
	if (_need_exit == false)
		std::cerr << "Error: Bad conf format" << std::endl;
	_need_exit = true;
}

void	Config::errorBadKeyword() {
	if (_need_exit == false) {
		std::cerr << "Cursor on : \"" << _word << "\"" << std::endl;
		std::cerr << "Error: Bad Key _word" << std::endl;
		std::cerr << "_word = " << _word << std::endl;
	}
	_need_exit = true;
}

void	Config::errorScopeDepth() {
	if (_need_exit == false) {
		std::cerr << "Cursor on : \"" << _word << "\"" << std::endl;
		std::cerr << "Error: Trying to go depth 3 scope" << std::endl;
	}
	_need_exit = true;
}

void	Config::errorBadCgi() {
	if (_need_exit == false)
		std::cerr << "Error: Bad cgi format" << std::endl;
	_need_exit = true;
}

void	Config::errorTooMuchCgi() {
	if (_need_exit == false)
		std::cerr << "Error: To much Cgi" << std::endl;
	_need_exit = true;
}

void	Config::printAllConfig() const {
	for (size_t j = 0; j < _config.size(); j++) {
		std::cout << "\nBlock Nb : " << j+1 << '\n' << std::endl;
		std::cout << "	port = " << _config.at(j).getPort() << std::endl;
		std::cout << "	server_name = " << _config.at(j).getServerName() << std::endl;
		std::cout << "	root = " << _config.at(j).getRoot() << std::endl;
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

			_config.at(j).getLocation().at(i).printCgiMap();
			for (size_t k = 0; k < _config.at(j).getLocation().at(i).getIndex().size(); k++) {
				if (k == 0)
					std::cout << "		index = ";
				std::cout << _config.at(j).getLocation().at(i).getIndex().at(k);
				if (i == _config.at(j).getLocation().at(i).getIndex().size() - 1)
					std::cout << std::endl;
				else
					std::cout << " ";
			}
		}
	}
}

void	Config::parsing() {
	if (_in_file) {
		bool	server_scope = false;
		int		location_scope = false;

		while (_in_file >> _word) {
			if (server_scope == false) {
				if (_word == "server") {
					this->addConfig();
					if (_word == "{") {
						_new_instruction = true;
						server_scope = true;
					} else {
						this->errorBadConf();
					}
				} else {
					this->errorBadConf();
				}
			} else {
				if (_word == "}" && location_scope == false) {
					server_scope = false;
//				} else if (_word == "}" && location_scope == true) {
//					location_scope = false;
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
								this->errorBadConf();
							}
						}
					} else {
						this->parsLocation(location_scope);
					}
				}
			}
		}
		this->printAllConfig();
	} else {
		this->errorCantReadFile();
		return ;
	}
	this->concatPath();
	this->setAllDefaultValue();
}

Config &Config::operator=(const Config &conf) {
	this->setNeedExit(conf._need_exit);
	this->setLastInstruction(conf._last_instruction);
	this->setWord(conf._word);
	this->setPath(conf._path);
	this->setBlockIndex(conf._block_index);
	this->setLocId(conf._loc_id);
	this->setBlockInstruction(conf._new_instruction);
	this->setConfig(conf._config);
	return *this;
}
