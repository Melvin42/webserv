#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "BlockConfig.hpp"
#include <vector>

class Config {
	public:
		Config();
		Config(const char *av);
		~Config();

		std::vector<BlockConfig>	getConfig() const;
		std::string					getPath() const;
		void						addConfig();

//		void						getInFile(const std::ifstream &in);
//		void						getLastInstruction(const std::string &instru);
//		void						getWord(const std::string &word);
//		void						getPath(const std::string &path);
//		void						getBlockIndex(const int &index);
//		void						getLocId(const int &id);
//		void						getBlockInstruction(const int &flag);
//		void						getConfig(const std::vector<BlockConfig> &conf);

//		void						setInFile(const std::ifstream in);;
		void						setLastInstruction(const std::string &instru);
		void						setWord(const std::string &word);
		void						setPath(const std::string &path);
		void						setBlockIndex(const int &index);
		void						setLocId(const int &id);
		void						setBlockInstruction(const int &flag);
		void						setConfig(const std::vector<BlockConfig> &conf);

		void						concatPath();

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

		Config &operator=(const Config &conf);
	private:
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
