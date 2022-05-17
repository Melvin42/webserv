#include "Config.hpp"

Config::Config()
	: _is_to_redirect(false), _need_exit(false), _check_binary("SCRIPT_EXT"),
	_last_instruction(""), _word(""),
	_block_index(-1), _loc_id(-1), _new_instruction(true) {
}

Config::Config(const Config &cp) {
	*this = cp;
}

Config::Config(const char *av)
	: _is_to_redirect(false), _need_exit(false), _check_binary("SCRIPT_EXT"),
	_last_instruction(""), _word(""),
	_block_index(-1), _loc_id(-1), _new_instruction(true) {
	this->parsing(av);
}

Config::~Config() {
}

void	Config::addConfig(std::ifstream &in_file) {
	BlockConfig	conf;

	_block_index++;
	_loc_id = -1;
	_config.push_back(conf);
	in_file >> _word;
}

std::vector<BlockConfig>	Config::getConfig() const {
	return _config;
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

void	Config::setAllDefaultValue() {
	for (size_t i = 0; i < _config.size(); i++) {
		_config.at(i).setDefaultIndex();
		_config.at(i).setId(i);
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
		|| word == "autoindex" || word == "404_default" || word == "rewrite"
		|| word == "client_body_size_max" || word == "disallow"
		|| word == "{" || word == "}")
		return true;
	return false;
}

void	Config::parsPort() {

	size_t	found;

	found = _word.find(":", 0);
	
	if (found < _word.size()) {
		_config.at(_block_index).setNewHost(_word.substr(0, found));
		_word = _word.substr(found + 1);
	}
	_word = this->badEndOfLine();
	for (size_t i = 0; i < _word.size(); i++) {
		if (!isdigit(_word[i]))
			this->errorBadConf();
	}
	int	tmp = atoi(this->_word.c_str());
	if (tmp < 2000 || tmp == 4242 || tmp > 65535)
		this->errorBadPort();
	_config.at(_block_index).setNewPort(tmp);
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
	this->checkSemiColon();
	if (_new_instruction)
		_config.at(_block_index).addIndexToLocation(this->checkEndOfLine(';'), _loc_id);
	else
		_config.at(_block_index).addIndexToLocation(_word, _loc_id);
}

void	Config::parsAutoindex() {
	std::string	tmp = this->badEndOfLine();

	if (tmp == "on") {
		_config.at(_block_index).setAutoindex(true);
	} else if (tmp == "off") {
		_config.at(_block_index).setAutoindex(false);
	} else {
		this->errorBadConf();
	}
}

void	Config::parsDefaultErrorPage() {
	_config.at(_block_index).setDefault404(this->badEndOfLine());
}

void	Config::parsClientMaxBodySize() {
	_word = this->badEndOfLine();

	for (size_t i = 0; i < _word.size(); i++) {
		if (!isdigit(_word[i]) || i > 18) {
			this->errorBadConf();
		}
	}
	_config.at(_block_index).setBodySizeMax(atoi(this->_word.c_str()));
}

void	Config::parsDisallow() {
	std::string	tmp = this->badEndOfLine();
	if (tmp == "POST") {
		_config.at(_block_index).setCanPost(false);
	} else if (tmp == "GET") {
		_config.at(_block_index).setCanGet(false);
	} else if (tmp == "DELETE") {
		_config.at(_block_index).setCanDelete(false);
	} else {
		this->errorBadConf();
	}
}

void	Config::parsRewrite() {
	if (_is_to_redirect) {
		_config.at(_block_index).setToRedirect(this->checkEndOfLine(';'));
		_is_to_redirect = false;
		std::cerr << "to_r" << std::endl;
	} else {
		_config.at(_block_index).setRedirectTo(this->badEndOfLine());
		std::cerr << "r_to" << std::endl;
	}
}

void	Config::parsCgi(std::ifstream &in_file) {
	std::string valuemap;

	in_file >> _word;
	if (_word == "BINARY" && _check_binary == "SCRIPT_EXT") {
		in_file >> _word;
		valuemap = this->badEndOfLine();
		_check_binary = "BINARY";
		in_file >> _word;
	}
	if (_word == "cgi-bin")
		in_file >> _word;
	if (_word == "SCRIPT_EXT" && _check_binary == "BINARY") {
		in_file >> _word;
		_check_binary = "SCRIPT_EXT";
		_config.at(_block_index).addCgiToLocationMap(_word, valuemap, _loc_id);
	} else {
		this->errorBadCgi();
	}
}

void	Config::parsLocation(std::ifstream &in_file, int &location_scope) {
	if (location_scope == false) {
		_config.at(_block_index).setNewLocation(_word);
		in_file >> _word;
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
				while (!_new_instruction && !in_file.eof()) {
					in_file >> _word;
					if (isInstruction(_word)) {
						this->errorBadConf();
						_new_instruction = true;
					}
					this->parsLocationIndex();
				}
			} else if (_word == "cgi-bin") {
				this->parsCgi(in_file);
			}
		} else {
			this->errorBadConf();
		}
	}
}

void	Config::errorNoSemiColon() {
	if (_need_exit == false)
		std::cerr << "Error: Semi Colon" << std::endl;
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

void	Config::errorBadPort() {
	if (_need_exit == false)
		std::cerr << "Error: Bad port" << std::endl;
	_need_exit = true;
}

void	Config::errorBadKeyword() {
	if (_need_exit == false) {
		std::cerr << "Error: Bad Key _word" << std::endl;
		std::cerr << "_word = " << _word << std::endl;
	}
	_need_exit = true;
}

void	Config::errorScopeDepth() {
	if (_need_exit == false) {
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
		std::cout << "	host = " << _config.at(j).getHost() << std::endl;
		std::cout << "	port = " << _config.at(j).getPort() << std::endl;
		std::cout << "	server_name = " << _config.at(j).getServerName() << std::endl;
		std::cout << "	root = " << _config.at(j).getRoot() << std::endl;
		std::cout << "	404_default = " << _config.at(j).getDefault404() << std::endl;
		std::cout << "	Toredirect = " << _config.at(j).getToRedirect() << std::endl;
		std::cout << "	redirectTo = " << _config.at(j).getRedirectTo() << std::endl;
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
				if (k == _config.at(j).getLocation().at(i).getIndex().size() - 1)
					std::cout << std::endl;
				else
					std::cout << " ";
			}
		}
		std::cerr << "	POST = " << _config.at(j).getCanPost() << std::endl;
		std::cerr << "	GET = " << _config.at(j).getCanGet() << std::endl;
		std::cerr << "	DEL = " << _config.at(j).getCanDelete() << std::endl;

		if (_config.at(j).getAutoindex())
			std::cerr << "	autoindex = " << "ON" << std::endl;
		else
			std::cerr << "	autoindex = " << "OFF" << std::endl;

		std::cerr << "	client_body_size_max = " << _config.at(j).getBodySizeMax() << std::endl;

	}
}

void	Config::saveLastInstruction() {
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
	} else if (_word == "autoindex") {
		_last_instruction = _word;
	} else if (_word == "404_default") {
		_last_instruction = _word;
	} else if (_word == "client_body_size_max") {
		_last_instruction = _word;
	} else if (_word == "disallow") {
		_last_instruction = _word;
	} else if (_word == "rewrite") {
		_last_instruction = _word;
		_is_to_redirect = true;
	} else if (_word == "location") {
		_loc_id++;
		_last_instruction = _word;
	}
}

void	Config::parsInstruction(std::ifstream &in_file, int &location_scope) {
	if (_last_instruction == "listen") {
		this->parsPort();
	} else if (_last_instruction == "server_name") {
		this->parsServerName();
	} else if (_last_instruction == "root") {
		this->parsRoot();
	} else if (_last_instruction == "index") {
		this->parsIndex();
	} else if (_last_instruction == "autoindex") {
		this->parsAutoindex();
	} else if (_last_instruction == "404_default") {
		this->parsDefaultErrorPage();
	} else if (_last_instruction == "client_body_size_max") {
		this->parsClientMaxBodySize();
	} else if (_last_instruction == "disallow") {
		this->parsDisallow();
	} else if (_last_instruction == "rewrite") {
		this->parsRewrite();
	} else if (_last_instruction == "location") {
		this->parsLocation(in_file, location_scope);
	}
	else {
		this->errorBadConf();
	}
}

void	Config::parsing(const char *av) {
	std::ifstream	in_file(av);

	if (in_file) {
		bool	server_scope = false;
		int		location_scope = false;

		while (in_file >> _word) {
			if (server_scope == false) {
				if (_word == "server") {
					this->addConfig(in_file);
					if (_word == "{") {
						_new_instruction = true;
						server_scope = true;
					} else
						this->errorBadConf();
				} else
					this->errorBadConf();
			} else {
				if (_word == "}" && location_scope == false) {
					server_scope = false;
//				} else if (_word == "}" && location_scope == true) {
//					location_scope = false;
				} else {
					if (location_scope == false) {
						if (_new_instruction == true)
							this->saveLastInstruction();
						else if (_new_instruction == false)
							this->parsInstruction(in_file, location_scope);
					} else
						this->parsLocation(in_file, location_scope);
				}
			}
		}
		this->printAllConfig();
	} else {
		this->errorCantReadFile();
		return ;
	}
	this->setAllDefaultValue();
}

Config &Config::operator=(const Config &conf) {
	_need_exit = conf._need_exit;
	_is_to_redirect = conf._is_to_redirect;
	_last_instruction = conf._last_instruction;
	_word = conf._word;
	_block_index = conf._block_index;
	_loc_id = conf._loc_id;
	_new_instruction = conf._new_instruction;
	_config = conf._config;
	return *this;
}
