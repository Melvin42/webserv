#include "HttpRequest.hpp"

HttpRequest::HttpRequest(void) : _method(""), _page(""),
	_version(""), _host(""), _body(""), _content_length(0) {
}

HttpRequest::HttpRequest(const char *buffer, int buf_size, const std::string &root) {
	std::stringstream	line;
//	std::cerr << buffer << std::endl;
	line << buffer;
	line >> _method >> _page >> _version;
	std::string tmp;
//	std::cerr << _page << std::endl;
	tmp = root + _page;
	_page = tmp;

	this->readRequest(buffer, buf_size);
}

HttpRequest::HttpRequest(const HttpRequest &httprequest) {
	*this = httprequest;
}

HttpRequest &HttpRequest::operator=(const HttpRequest &httprequest) {
	_method = httprequest._method;
	_page = httprequest._page;
	_version = httprequest._version;
	_host = httprequest._host;
	_content_length = httprequest._content_length;
	_body = httprequest._body;
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

std::string	HttpRequest::getBody() const {
	return _body;
}

size_t	HttpRequest::getContentLength() const {
	return _content_length;
}

void	HttpRequest::readRequest(const char *buffer, int buf_size) {
	size_t		found = 0;
	int			i = -1;
	std::string	req = "";

	req += buffer;
	found = req.find("Host: ");
	i = found + 5;
	while (++i < buf_size && buffer[i] != '\n')
		_host += buffer[i];
	found = req.find("Content-Length: ");
	if (i < buf_size) {
		i = found + 15;
		_content_length = 0;
		while (++i < buf_size && buffer[i] != '\n' && isdigit(buffer[i])) {
			_content_length *= 10;
			_content_length += (buffer[i] - 48);
		}
	}
	while (++i < buf_size && !(buffer[i] == '\n' && buffer[i - 1] == '\r'
				&& buffer[i - 2] == '\n' && buffer[i - 3] == '\r'))
		;
	while (++i < buf_size && i != '\0')
		_body += buffer[i];
}

