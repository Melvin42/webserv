#include "HttpRequest.hpp"

HttpRequest::HttpRequest() {
}

HttpRequest::HttpRequest(const char *buffer, const BlockConfig &conf): _conf(conf) {
	std::stringstream	line;

//	std::cerr << "
	line << buffer;
	line >> _request["method"];
	line >> _request["page"];
	line >> _request["version"];
	line.ignore();
	setFullPage();
	parseHeader(line);
	if (_request["method"] == "POST")
		postCheck(line);
	std::cout << "fullPage: " << _request["fullpage"] << std::endl;
	std::cout << "_request[\"posted\"]: " << _request["posted"] << std::endl;
	std::cout << std::endl << "line: " << std::endl << line.str();
}

HttpRequest::HttpRequest(const HttpRequest &httprequest) {
	*this = httprequest;
}

HttpRequest &HttpRequest::operator=(const HttpRequest &httprequest) {
	_request = httprequest._request;
	return *this;
}

HttpRequest::~HttpRequest() {
}

std::string	HttpRequest::getMethod() {
	return _request["method"];
}

std::string	HttpRequest::getFullPage() {
	return _request["fullpage"];
}

std::string	HttpRequest::getVersion() {
	return _request["version"];
}

std::string	HttpRequest::getHost() {
	return _request["Host"];
}

size_t	HttpRequest::getContentLength(){
	return std::atoi(_request["Content-Length"].c_str());
}

std::map<std::string, std::string> HttpRequest::getRequest() const {
	return _request;
}

void	HttpRequest::setFullPage() {
	if (_request["page"] == "/")
		_request["fullpage"] = _conf.getDefaultIndex();
	else if (_request["page"] == _conf.getToRedirect())
        _request["fullpage"] = _conf.getRedirectTo();
	else
		_request["fullpage"] = _conf.getRoot() + _request["page"];
}

void	HttpRequest::postCheck(std::stringstream &line) {

	if (_request.find("Content-Length") == _request.end())
		_request["posted"] = "noCL";
	else if (_conf.getBodySizeMax() > 0 && getContentLength() > _conf.getBodySizeMax())
	{
		_request["posted"] = "tooBig";
		 	return ;
	}
	else if (_request.find("boundary") != _request.end() && _conf.getCanPost())
		parseBody(line);
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

	while (std::getline(line, buf)) {
		if (line.eof() || line.bad() || buf == _request["boundary"])
			break ;
	}
	while (42) {
		if (line.eof() || line.bad() || buf == (_request["boundaryEnd"]))
			break ;
		else if (buf != _request["boundary"])
			std::getline(line, buf);
		if (buf == _request["boundary"])
		{
			while (std::getline(line, buf)) {
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
// std::cout << buf << std::endl ;
				if (line.eof() || line.bad() || buf == "\r"
					|| buf == _request["boundary"] || buf == _request["boundaryEnd"])
				{
					filest.close();
					break ;
				}
				filest << buf;
				if (filest.fail())
					_request["posted"] = "false";
				if (line.peek() != '\0')
					filest << std::endl;
				else
					filest << std::ends;
			}
		}
	}
	if (!line.bad() && _request["posted"] != "false")
		_request["posted"] = "true";
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
	if (*_request["fullpage"].end() -1 != '/')
		filename = _request["fullpage"] + '/' + filename;
	else
		filename = _request["fullpage"] + filename;
	return filename;
}
