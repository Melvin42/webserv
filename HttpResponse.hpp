#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include <sstream>
#include <fstream>
#include <string>
#include <iostream>
#include <map>

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
};

#endif
