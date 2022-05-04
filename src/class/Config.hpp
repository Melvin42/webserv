#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "BlockConfig.hpp"
#include <vector>

class Config {
	public:
		Config(const char *av);
		~Config();

		std::vector<BlockConfig>	getConfig() const ;
		void						addConfig();

		std::string					checkEndOfLine(const std::string &word, char c);
		void						parsing(const char *av);
		void						parsPort(const std::string &word, int block_index);
		void						parsServerName(const std::string &word, int block_index);
		void						parsRoot(const std::string &word, int block_index);
		void						parsLocation(const std::string &word, int block_index);

		void	printAllConfig() const;

	private:
		Config();
		std::vector<BlockConfig>	_config;
};

#endif
