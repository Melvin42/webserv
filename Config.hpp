#ifndef CONFIG_HPP
#define CONFIG_HPP

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

		void	printMap() const;
		void	parsing(const char *av);

	private:
		Config();
		std::map<std::string, std::string>	_config;
//		int	_port;
};

#endif
