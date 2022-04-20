#ifndef BLOCKCONFIG_HPP
#define BLOCKCONFIG_HPP

#include "BlockConfig.hpp"
#include <vector>

class Config {
	public:
		Config(const char *av);
		Config(const Config &config);
		~Config();

	private:
		Config();
		std::vector<BlockConfig>	_config;
}

#endif
