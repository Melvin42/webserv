#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "BlockConfig.hpp"
#include <vector>

class Config {
	public:
		Config(const char *av);
		~Config();

		std::vector<BlockConfig>	getConfig() const;
		void						addConfig();

		std::string	checkEndOfLine(char c);
		void		checkSemiColon();

		void		parsing();
		void		parsPort();
		void		parsServerName();
		void		parsRoot();
		void		parsIndex();
		void		parsLocation(int &location_scope);
		void		parsServerScope();

		void		errorBadConf() const;
		void		errorBadKeyword() const;
		void		errorScopeDepth() const;

		void	printAllConfig() const;

	private:
		Config();
		std::ifstream				_in_file;
		std::string					_last_instruction;
		std::string					_word;
		int							_block_index;
		int							_loc_id;
		bool						_new_instruction;
		std::vector<BlockConfig>	_config;
};

bool	isInstruction(const std::string &word);

#endif
