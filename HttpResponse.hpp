#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include <sstream>
#include <fstream>
#include <string>
#include <iostream>
#include <map>
#include <vector>

class	HttpResponse {
	public:

		HttpResponse(void);
		HttpResponse(const HttpResponse &httpresponse);
		~HttpResponse(void);
		HttpResponse &operator=(const HttpResponse &httpresponse);

		std::string	getHttpResponse(std::string path);
		std::vector<char>	postHttpResponse(std::string path);

	private:

		std::string							_ret_get;
		std::vector<char>					_ret_post;
		std::map<std::string, std::string>	_status;
};

#endif
