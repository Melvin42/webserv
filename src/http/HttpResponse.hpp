// #ifndef HTTPRESPONSE_HPP
// #define HTTPRESPONSE_HPP

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

#include "../config/Config.hpp"

class	HttpResponse {
	public:

		
		HttpResponse(char **env, std::map<std::string, std::string> request);
		~HttpResponse();

		std::string	getHttpResponse();

	private:

		char								**_env;
		char								**_exec_argv;
		std::string							_ret;
		std::map<std::string, std::string>	_cgi;
		std::map<std::string, std::string>	_status;
		std::map<std::string, std::string>	_request;
		

		HttpResponse();
		int		is_cgi();
		void	statusRet(std::string errCode);
		void	set_exec_argv(std::string cmdPath, std::string errPage);
		int		cgi(std::string statusKey);
		void	setHeader(std::string statusKey);
		void	setCgiHeader(std::string statusKey);
		void	setPage(std::string statusKey, std::ifstream &page);
		void	autoIndex();

		void	methodGetRes();
		void	methodPostRes();
		void	methodDeleteRes();
		// std::string	interface(std::string errorCode);
};

// #endif
