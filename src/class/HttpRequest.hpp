#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <ctype.h>
#include "ClientManager.hpp"

class	HttpRequest {
	public:

		HttpRequest(void);
		HttpRequest(const char *buffer, int buf_size);
		HttpRequest(const HttpRequest &httprequest);
		~HttpRequest(void);
		HttpRequest &operator=(const HttpRequest &httprequest);

		std::string	getMethod() const;
		std::string	getPage() const;
		std::string	getVersion() const;
		std::string	getHost() const;
		std::string	getBody() const;
		size_t	getContentLength() const;

	private:

		std::string	_method;
		std::string	_page;
		std::string	_version;
		std::string	_host;
		std::string	_body;
		size_t	_content_length;
};

#endif
