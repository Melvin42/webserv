#ifndef BLOCKCONFIG_HPP
#define BLOCKCONFIG_HPP

#include <iostream>
#include <fstream>
#include <inttypes.h>
#include <map>
#include <algorithm>
#include <vector>

class BlockConfig {
	public:
		BlockConfig(const char *av);
		BlockConfig(const BlockConfig &config);
		~BlockConfig();

		std::map<std::string, std::string>	getConfig() const;

		std::vector<int>			getPort() const;
		std::vector<std::string>	getServerName() const;
		std::vector<std::string>	getRoot() const;
		std::vector<std::string>	getLocation() const;
		std::string					checkEndOfLine(std::string word);
		void						parsing(const char *av);

	private:
		BlockConfig();
		std::vector<int>			_port;
		std::vector<std::string>	_server_name;
		std::vector<std::string>	_root;
		std::vector<std::string>	_location;
};

#endif
