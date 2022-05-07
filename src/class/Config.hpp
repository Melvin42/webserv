#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "BlockConfig.hpp"
#include <vector>

class Config {
	public:
		Config(const char *av);
		~Config();

		std::vector<BlockConfig>	getConfig() const;
		std::string					addConfig(std::ifstream &stream,
				int &block_index, int &loc_id);
//
		std::string	checkEndOfLine(const std::string &word, char c);
		void		parsing(const char *av);
		void		parsPort(std::ifstream &stream, int block_index);
		void		parsServerName(std::ifstream &stream, int block_index);
		void		parsRoot(std::ifstream &stream, int block_index);
		void		parsLocation(std::ifstream &stream, int block_index,
				int loc_id, int &location_scope);
		void		parsServerScope(std::ifstream &stream);
//		void		parsLocationScope(std::s);

		void		errorBadConf() const;
		void		errorBadKeyword(const std::string &word) const;
		void		errorScopeDepth(const std::string &word) const;

		void	printAllConfig() const;

	private:
		Config();
		std::vector<BlockConfig>	_config;
};

#endif
