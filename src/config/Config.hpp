#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "BlockConfig.hpp"
#include <vector>

class Config {
	public:
		Config();
		Config(const char *av);
		Config(const Config &cp);
		~Config();

		std::vector<BlockConfig>	getConfig() const;
		void						addConfig(std::ifstream &in_file);

		bool						getNeedExit() const;

		void						setNeedExit(const bool &need_exit);
		void						set(const bool &need_exit);
		void						setLastInstruction(const std::string &instru);
		void						setWord(const std::string &word);
		void						setBlockIndex(const int &index);
		void						setLocId(const int &id);
		void						setBlockInstruction(const int &flag);
		void						setConfig(const std::vector<BlockConfig> &conf);

		void						setAllDefaultValue();
		void						setAllDefaultServer();

		std::string	checkEndOfLine(char c);
		std::string	badEndOfLine();
		void		checkSemiColon();

		void		parsing(const char *av);
		void		saveLastInstruction();
		void		parsInstruction(std::ifstream &in_file, int &location_scope);
		void		parsPort();
		void		parsServerName();
		void		parsRoot();
		void		parsIndex();
		void		parsAutoindex();
		void		parsDefaultErrorPage();
		void		parsClientMaxBodySize();
		void		parsDisallow();
		void		parsRewrite();
		void		parsLocationIndex();
		void		parsLocation(std::ifstream &in_file, int &location_scope);
		void		parsServerScope();
		void		parsCgi(std::ifstream &in_file);

		void		errorBadConf();
		void		errorBadPort();
		void		errorCantReadFile();
		void		errorNoSemiColon();
		void		errorBadKeyword();
		void		errorScopeDepth();
		void		errorBadCgi();
		void		errorTooMuchCgi();

		void	printAllConfig() const;

		Config &operator=(const Config &conf);
	private:
		bool						_is_to_redirect;
		bool						_need_exit;
		std::string					_check_binary;
		std::string					_last_instruction;
		std::string					_word;
		int							_block_index;
		int							_loc_id;
		bool						_new_instruction;
		std::vector<BlockConfig>	_config;
};

bool	isInstruction(const std::string &word);

#endif
