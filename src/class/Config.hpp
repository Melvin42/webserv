#ifndef CONFIG_HPP
#define CNFIG_HPP

#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
#include <vector>

class Config {
	public:
		Config(const char *av);
		Config(const Config &config);
		~Config();

		std::map<std::string, std::string>	getConfig() const;

		int		getPort() const;
		void	printMap() const;
		void	parsing(const char *av);

	private:
		Config();
		std::map<std::string, std::string>	_config;
		std::vector<std::string>			_root;
		std::vector<std::string>			_location;
		std::string							_server_name;
		int									_port;
};

#endif
