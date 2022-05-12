#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <ctype.h>
#include <limits>
#include <stdlib.h>
#include "../server/ClientManager.hpp"

class	HttpRequest {
	public:

		HttpRequest(const char *buffer, int buf_size, const std::string &root);
		HttpRequest(const HttpRequest &httprequest);
		~HttpRequest();
		HttpRequest &operator=(const HttpRequest &httprequest);

		std::string	getMethod();
		std::string	getPage();
		std::string	getVersion();
		std::string	getHost();
		std::string	getBody();
		size_t	getContentLength();

		void	parseHeader(std::stringstream &line);
		void	parseBody(std::stringstream &line);
	private:

		HttpRequest();
		std::map<std::string, std::string>				_request;
		std::string	_body;
};

#endif
