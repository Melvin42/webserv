#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "BlockConfig.hpp"
#include <vector>

class Config {
	public:
		Config();
		Config(const char *av);
		Config(Config &cp);
		~Config();

		std::vector<BlockConfig>	getConfig() const;
		std::string					getPath() const;
		void						addConfig();

		bool						getNeedExit() const;

		void						setNeedExit(const bool &need_exit);
		void						setLastInstruction(const std::string &instru);
		void						setWord(const std::string &word);
		void						setPath(const std::string &path);
		void						setBlockIndex(const int &index);
		void						setLocId(const int &id);
		void						setBlockInstruction(const int &flag);
		void						setConfig(const std::vector<BlockConfig> &conf);

		void						concatPath();
		void						setAllDefaultValue();


		std::string	checkEndOfLine(char c);
		std::string	badEndOfLine();
		void		checkSemiColon();

		void		parsing();
		void		parsPort();
		void		parsServerName();
		void		parsRoot();
		void		parsIndex();
		void		parsCgiBinary();
		void		parsCgiFilename();
		void		parsLocationIndex();
		void		parsLocation(int &location_scope);
		void		parsServerScope();
		void		parsCgi();

		void		errorBadConf();
		void		errorCantReadFile();
		void		errorNoSemiColon();
		void		errorBadKeyword();
		void		errorScopeDepth();
		void		errorBadCgi();
		void		errorTooMuchCgi();

		void	printAllConfig() const;

		Config &operator=(const Config &conf);
	private:
		bool						_need_exit;
		std::string					_check_binary;
		std::ifstream				_in_file;
		std::string					_last_instruction;
		std::string					_word;
		std::string					_path;
		int							_block_index;
		int							_loc_id;
		bool						_new_instruction;
		std::vector<BlockConfig>	_config;
};

bool	isInstruction(const std::string &word);

#endif
