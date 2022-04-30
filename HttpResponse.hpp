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

class	HttpResponse {
	public:

		HttpResponse(char **env);
		HttpResponse(const HttpResponse &httpresponse);
		~HttpResponse(void);
		HttpResponse &operator=(const HttpResponse &httpresponse);

		std::string	getHttpResponse(std::string requestedPagePath);

	private:

		std::string							_ret;
		std::map<std::string, std::string>	_status;
		char **								_env;


		HttpResponse(void);

		int cgi_parcing_check(std::string requestedPagePath);
		std::string get_cmd_bin(std::string cmd) const;
		char ** get_execve_argv(std::string requestedPagePath, std::string cmd) const;
		int cgi(char ** execve_argv);
		void getPage(std::ifstream	&page, std::string statusKey);
		// std::string	interface(std::string errorCode);
		std::string	_404NotFound(void);

};

#endif
