#pragma once

#include <sstream>
#include <fstream>
#include <string>
#include <cstring>
#include <iostream>
#include <map>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <ios>
#include <dirent.h>
#include <cctype>
	

#include "../config/Config.hpp"

class	HttpResponse {
	public:

		HttpResponse(BlockConfig config, std::map<std::string, std::string> request);
		~HttpResponse();
		std::string	getHttpResponse();

	private:

		char								**_env;
		char								**_exec_argv;
		BlockConfig 						_conf;
		std::string							_ret;
		std::map<std::string, std::string>	_cgi;
		std::map<std::string, std::string>	_status;
		std::map<std::string, std::string>	_request;
		

		HttpResponse();
		void	initStatus();
		void	initCgi();

		void	methodGet();
		void	methodPost();
		void	methodDelete();

		int		is_cgi(const std::string &uri);
		bool	findCgi();
		int		cgi(std::string statusKey);
		std::map<std::string, std::string>    initEnv();
		void	getEnv();
		std::string	toUpper(std::string str);
		void	set_exec_argv(const std::string &cmdPath, const std::string &param);

		void	setHeader(std::string statusKey);
		void	setCgiHeader(std::string statusKey);
		void	setPage(std::string statusKey, std::ifstream &page);

		void	autoIndex();
		void	statusRet(std::string errCode);

};
