#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

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

class	HttpResponse {
	public:

		
		HttpResponse(char **env);
		~HttpResponse();

		std::string	getHttpResponse(std::string requestedPagePath);

	private:

		std::string							_ret;
		std::string							_root;
		std::string							_full_path;
		std::map<std::string, std::string>	_status;
		std::map<std::string, std::string>	_cgi;
		char								**_env;
		char								**_exec_argv;

		HttpResponse();
		int		is_cgi(std::string requestedPagePath);
		void	errCgi(std::string errCode);
		void	set_exec_argv(std::string requestedCgiPath, std::string cmdPath, std::string errPage);
		int		cgi();
		void	getHeader(std::string statusKey);
		void	getPage(std::ifstream &page);
		// std::string	interface(std::string errorCode);
};

#endif
