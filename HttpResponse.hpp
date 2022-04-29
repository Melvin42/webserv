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

class	HttpResponse {
	public:

		HttpResponse(void);
		HttpResponse(const HttpResponse &httpresponse);
		~HttpResponse(void);
		HttpResponse &operator=(const HttpResponse &httpresponse);

		std::string	getHttpResponse(std::string path);

	private:

		std::string							_ret;
		std::map<std::string, std::string>	_status;

		int cgi(std::string path);
		// std::string	interface(std::string errorCode);
		std::string	_404NotFound(void);

};

#endif
