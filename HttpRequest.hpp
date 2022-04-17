#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

class	HttpRequest {
	public:

		HttpRequest(void);
		HttpRequest(char *buffer, buf_size);
		HttpRequest(const HttpRequest &httprequest);
		~HttpRequest(void);
		HttpRequest &operator=(const HttpRequest &httprequest);

	private:

		std::string	_method;
		std::string	_page;
		std::string	_version;
		std::string	_host;
};

#endif
