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
	line.ignore();
	parseHeader(line);
	if (_request.find("boundary") != _request.end() /*&& conf allow upload*/)
		parseBody(line);
	// std::cout << std::endl << "line: " << std::endl << line.str();
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

void	HttpRequest::parseHeader(std::stringstream &line) {
	std::string buf;
	std::string key;
	std::string value;
	line.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	while (std::getline(line, buf))
	{
		if (line.eof() || line.bad() || buf == "\r")
			break ;
		key = getKey(buf);
		value = getValue(buf);
		if (key.find("Content-Type") != std::string::npos && value.find("boundary=") != std::string::npos)
		{
			_request["boundary"] = "--" + value.substr(value.find("boundary=") + 9) + "\r";
			_request["boundaryEnd"] = "--" + value.substr(value.find("boundary=") + 9) + "--";
			value.erase(value.find(";"));
		}
		_request[key] = value;
	}
}

void	HttpRequest::parseBody(std::stringstream &line) {
	std::string							buf;
	std::string							key;
	std::string							value;
	std::string							filename;
	std::map<std::string, std::string>	bodyHeader;
	std::FILE*							file;

// std::FILE* tmpf = fopen("/tmp/.tmp", "wb+");
// (void)tmpf;
// std::ofstream tmpst("/tmp/.tmp");
// int i = 0;

	while (std::getline(line, buf)) {
		// tmpst << "buf is   :" << buf << std::endl;
		// tmpst << "boundary :" << _request["boundary"] << std::endl;
		// tmpst << "i = " << i << std::endl;
		if (line.eof() || line.bad() || buf == _request["boundary"])
			break ;
		// i++;
	}
	while (42) {
		if (line.eof() || line.bad() || buf == _request["boundaryEnd"])
			break ;
		if (buf == _request["boundary"])
		{
			while (std::getline(line, buf)) {
				// std::cout << "buf is :" << buf << std::endl;
				if (line.eof() || line.bad() || buf == "\r")
					break ;
				bodyHeader[getKey(buf)] = getValue(buf);
			}
			if (line.eof() || line.bad())
				break ;
			if (buf == "\r") {
				filename = getFilename(bodyHeader);
				file = fopen(filename.c_str(), "wb+");	
			}
			std::ofstream	filest(filename.c_str());
			while (std::getline(line, buf))
			{
				// std::cout << "buf is :" << buf << std::endl;
				if (line.eof() || line.bad() || buf.find("\r") != std::string::npos
					|| buf.find(_request["boundary"]) != std::string::npos
					|| buf.find(_request["boundaryEnd"]) != std::string::npos)
				{
					filest.close();
					break ;
				}
				// std::cout << "deal with content" << std::endl;
				filest << buf;
				if (line.peek() != '\0')
					filest << std::endl;
			}
		}
	}
}

std::string		HttpRequest::getKey(std::string buf) {
	return buf.substr(0, buf.find(":"));
}

std::string		HttpRequest::getValue(std::string buf) {
	std::string value;
	value = buf.substr(buf.find(":") + 1, buf.size());
	value.erase(std::remove(value.begin(), value.begin() + 1, ' '), value.begin() + 1);
	value.erase(std::remove(value.begin(), value.end(), '\r'), value.end());
	return value;
}

std::string		HttpRequest::getFilename(std::map<std::string, std::string>	&bodyHeader) {
	std::string		filename;
	if (bodyHeader["Content-Disposition"].find(" filename=") != std::string::npos)
	{
		filename = bodyHeader["Content-Disposition"].substr(
		bodyHeader["Content-Disposition"].find(" filename=") + 11,
		bodyHeader["Content-Disposition"].size());
	} else
	{
		filename = bodyHeader["Content-Disposition"].substr(
		bodyHeader["Content-Disposition"].find(" name=") + 7,
		bodyHeader["Content-Disposition"].size());
	}
	filename.erase(std::remove(filename.begin(), filename.end(), '\"'), filename.end());
	if (*_request["page"].end() -1 != '/')
		filename = _request["page"] + '/' + filename;
	else
		filename = _request["page"] + filename;
	return filename;
}