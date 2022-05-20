#include "HttpRequest.hpp"

HttpRequest::HttpRequest() {
}

HttpRequest::HttpRequest(std::string buffer, const BlockConfig &conf): _conf(conf) {
	std::stringstream	line;

	line << buffer;
	line >> _request["method"];
	line >> _request["page"];
	std::cerr << _request["page"]  << std::endl;
	line >> _request["version"];
	line.ignore();
	setFullPage();
	parseHeader(line);
	_request["body"] = buffer.substr(buffer.find("\r\n\r\n") + 4);
	if (_request["method"] == "POST")
		postCheck(line);
	if (_request["method"] == "DELETE")
		deleteCheck();
	// std::cout << "fullPage: " << _request["fullpage"] << std::endl;
	// std::cout << "_request[\"posted\"]: " << _request["posted"] << std::endl;
	// std::cout << std::endl << "line: " << std::endl << line.str();
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

size_t	HttpRequest::getContentLength(){
	return std::atoi(_request["content-length"].c_str());
}

std::map<std::string, std::string> HttpRequest::getRequest() const {
	return _request;
}

void	HttpRequest::setFullPage() {

	if (*(_request["page"].end() - 1) == '/')
		_request["fullpage"] = findIndex();
	else if (_request["page"] == _conf.getToRedirect())
		_request["fullpage"] = _conf.getRedirectTo();

	// This is the way we intent to handle redirections,
	// we will go further by looking in vector of redirections
	// this way the user will be able to set up multiple
	// redirections in the conf file 

	else
		_request["fullpage"] = _conf.getRoot() + _request["page"];
	// std::cerr << "full " << _request["fullpage"] << std::endl;
}

std::string	HttpRequest::findIndex() {
	// std::cerr << "finding ....." << std::endl;
	std::string	tmp;
	for (size_t i = 0; i < _conf.getIndex().size(); i++) {
		
		tmp = _conf.getRoot() + _request["page"].substr( _request["page"].find_first_of('/') + 1, _request["page"].size()) + _conf.getIndex().at(i);
std::cerr << tmp  << std::endl;
		if (access(tmp.c_str(), R_OK) == 0) {
			
			return tmp;
		}
	}
	tmp = _conf.getRoot() + _request["page"];
	return tmp;
}

void	HttpRequest::postCheck(std::stringstream &line) {

	if (_request.find("content-length") == _request.end())
		_request["posted"] = "411";
	else if (_conf.getBodySizeMax() > 0 && getContentLength() > _conf.getBodySizeMax())
	{
		_request["posted"] = "413";
		 	return ;
	}
	else if (_request.find("boundary") != _request.end() && _conf.getCanPost())
		parseBody(line);
}

void	HttpRequest::deleteCheck() {
	int ret = 0;
	std::ifstream	page(_request["fullpage"].c_str());
	if (!page)
	{
		_request["deleted"] = "404";
		return ;
	}
	ret = std::remove(_request["fullpage"].c_str());
	if (ret == 0)
		_request["deleted"] = "200";
	if (ret == -1)
		_request["deleted"] = "424";
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
		if (key.find("content-type") != std::string::npos && value.find("boundary=") != std::string::npos)
		{
			_request["boundary"] = "--" + value.substr(value.find("boundary=") + 9);
			_request["boundary-end"] = "--" + value.substr(value.find("boundary=") + 9) + "--";
			value.erase(value.find(";"));
		}
		_request[key] = value;
	}
}

void	HttpRequest::parseBody(std::stringstream &buf) {
	std::string							body;
	std::string							line;
	std::stringstream					tmp;
	std::string							key;
	std::string							value;
	std::string							filename;
	std::map<std::string, std::string>	bodyHeader;
	std::FILE*							file;

	(void)file;
	body = buf.str();

	std::cerr << body.size() << "...\n";
	if (body.find(_request["boundary"]) != std::string::npos)
	{
		while (42)
		{
			if (body.find(_request["boundary"]) == body.find(_request["boundary-end"]))
				break ;
			body = body.substr(body.find(_request["boundary"]) + _request["boundary"].size() + 2, body.size());
			tmp << body;
			// std::cerr << body << "...\n";
			while (std::getline(tmp, line)) { std::cerr << line << "\n";
				if (tmp.eof() || tmp.bad() || line == "\r"|| line == _request["boundary-end"])
					break ;
				
				bodyHeader[getKey(line)] = getValue(line);
			}
			if (tmp.eof() || tmp.bad())
					break ;
			if (line == "\r") {
					filename = getFilename(bodyHeader);
					file = fopen(filename.c_str(), "wb+");
				}
			std::ofstream	filest(filename.c_str());
			line = body.substr(body.find("\r\n\r\n") + 4, body.size());
			// std::cerr << line.size() << " bs: "<< body.size() << "...\n";

			// std::cerr << line << "...\n";
			line = line.substr(0, line.find(_request["boundary"]) - 2);
			// std::cerr << line.size() << "...\n";
			// std::cerr << line << "...\n";
			filest << line;
			filest.close();
		}
		if (tmp.bad())
			_request["posted"] = "424";
		else
			_request["posted"] = "201";
	}
	
}

std::string		HttpRequest::getKey(std::string buf) {
	return toLower(buf.substr(0, buf.find(":")));
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

std::string		HttpRequest::toLower(std::string str) {
	std::string::iterator	it;

	for (it = str.begin(); it !=str.end(); ++it)
		*it = std::tolower(*it);
	return str;
}
