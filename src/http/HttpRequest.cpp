#include "HttpRequest.hpp"

HttpRequest::HttpRequest() {
}

HttpRequest::HttpRequest(const char *buffer, int buf_size, const std::string &root) {
	std::stringstream	line;

	line << buffer;
	line >> _request["method"];
	line >> _request["page"];
	line >> _request["version"];
	_request["page"] = root + _request["page"];
	// std::cout << std::endl << "line: " << std::endl << line.str();
	line.ignore();
	parsing(line);
	(void)buf_size;
}

HttpRequest::HttpRequest(const HttpRequest &httprequest) {
	*this = httprequest;
}

HttpRequest &HttpRequest::operator=(const HttpRequest &httprequest) {
	_request = httprequest._request;
	_body = httprequest._body;
	return *this;
}

HttpRequest::~HttpRequest() {
}

std::string	HttpRequest::getMethod() {
	return _request["method"];
}

std::string	HttpRequest::getPage() {
	return _request["page"];
}

std::string	HttpRequest::getVersion() {
	return _request["version"];
}

std::string	HttpRequest::getHost() {
	return _request["Host"];
}

std::string	HttpRequest::getBody() {
	return _body;
}

size_t	HttpRequest::getContentLength(){
	return std::atoi(_request["Content-Length"].c_str());
}

void	HttpRequest::parsing(std::stringstream &line) {
	std::string key;
	std::string value;
	line.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	while (std::getline(line, key, ':'))
	{
		if (line.eof() || line.bad())
			break ;
		std::getline(line, value);
		std::remove(value.begin(), value.end(), ' ');
		value.erase(std::remove(value.begin(), value.end(), '\r'), value.end());
		_request[key] = value;
		
	}
	
	// req += buffer;
	// found = req.find("Content-Length: ");
	// i = found + 15;
	// while (++i < buf_size && buffer[i] != '\n' && isdigit(buffer[i])) {
	// 	_content_length *= 10;
	// 	_content_length += (buffer[i] - 48);
	// }
	// while (++i < buf_size && !(buffer[i] == '\n' && buffer[i - 1] == '\r'
	// 			&& buffer[i - 2] == '\n' && buffer[i - 3] == '\r'))
	// 	;
	// while (++i < buf_size && i != '\0')
	// 	_body += buffer[i];
}

