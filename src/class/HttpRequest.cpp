#include "HttpRequest.hpp"

HttpRequest::HttpRequest(void) : _method("GET"), _page("index.html"), 
	_version("HTTP/1.1"), _host("localhost:8080") {
}

HttpRequest::HttpRequest(char *buffer, int buf_size) {
	int	i = -1;
	while (buffer[++i] != ' ' && i < buf_size) {
		_method += buffer[i];
	}
	while (buffer[++i] != '/')
		;
	while (buffer[++i] != ' ' && i < buf_size) {
		_page += buffer[i];
	}
	while (buffer[++i] == ' ')
		;
	while (buffer[i] != '\n' && i < buf_size) {
		_version += buffer[i];
		i++;
	}
	if (i + 5 >= buf_size)
		return ;
	if (buffer[++i] == 'H' && buffer[i + 1] == 'o' && buffer[i + 2] == 's'
			&& buffer[i + 3] == 't' && buffer[i + 4] == ':') {
		i += 4;
		while (buffer[++i] == ' ')
			;
		while (buffer[i] != '\n' && i < buf_size) {
			_host += buffer[i];
			i++;
		}
	}
}

HttpRequest::HttpRequest(const HttpRequest &httprequest) {
	*this = httprequest;
}

HttpRequest &HttpRequest::operator=(const HttpRequest &httprequest) {
	_method = httprequest._method;
	_page = httprequest._page;
	_version = httprequest._version;
	_host = httprequest._host;
	return *this;
}

HttpRequest::~HttpRequest(void) {
}

std::string	HttpRequest::getMethod() const {
	return _method;
}

std::string	HttpRequest::getPage() const {
	return _page;
}

std::string	HttpRequest::getVersion() const {
	return _version;
}

std::string	HttpRequest::getHost() const {
	return _host;
}
