#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include <iostream>

class	HttpRequest {
	public:

		HttpRequest(void);
		HttpRequest(char *buffer, int buf_size);
		HttpRequest(const HttpRequest &httprequest);
		~HttpRequest(void);
		HttpRequest &operator=(const HttpRequest &httprequest);

		std::string	getMethod() const;
		std::string	getPage() const;
		std::string	getVersion() const;
		std::string	getHost() const;

	private:

		std::string	_method;
		std::string	_page;
		std::string	_version;
		std::string	_host;
};

#endif
