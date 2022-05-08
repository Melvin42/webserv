#include "HttpRequest.hpp"

HttpRequest::HttpRequest(void) : _method(""), _page(""), 
	_version(""), _host(""), _body(), _content_length(0) {
}

HttpRequest::HttpRequest(const char *buffer, int buf_size) {
	size_t	found = 0;
	std::stringstream	line;
	line << buffer;
	std::string	req = "";
	req += buffer;
	int	i = -1;
	line >> _method >> _page >> _version;
	std::string	tmp = _page.substr(1, _page.size());
	_page = tmp;
	std::cerr << "method: " << _method << " page: " << _page
		<< " version: " << _version << std::endl;
/*		while (buffer[++i] != ' ' && i < buf_size) {
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
	}*/
	found = req.find("Host: ");
	i = found + 5;
	while (++i < buf_size && buffer[i] != '\n')
		_host += buffer[i];
	std::cerr << "host: " << _host << std::endl;
/*		if (i + 5 >= buf_size)
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
	}*/
	found = req.find("Content-Length: ");
	if (i < buf_size) {
		i = found + 15;
		_content_length = 0;
		while (++i < buf_size && buffer[i] != '\n' && isdigit(buffer[i])) {
			_content_length *= 10;
			_content_length += (buffer[i] - 48);
		}
	}
	std::cerr << "content length: " << _content_length << std::endl;
	while (++i < buf_size && !(buffer[i] == '\n' && buffer[i - 1] == '\r'
				&& buffer[i - 2] == '\n' && buffer[i - 3] == '\r'))
		;
	while (++i < buf_size && i != '\0')
		_body += buffer[i];
	std::cerr << "body: " << _body
		<< " size: " << _body.size() << std::endl;
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
