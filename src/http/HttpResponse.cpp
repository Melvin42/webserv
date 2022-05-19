#include "HttpResponse.hpp"

HttpResponse::HttpResponse() {
}

HttpResponse::HttpResponse(BlockConfig config, std::map<std::string, std::string> request)
 : _env(NULL), _exec_argv(NULL), _conf(config), _ret(""), _request(request) {

	initStatus();
	// initCgi();
	//tmply be like this, _cgi should be configured by input .conf file
	_cgi.insert(std::pair<std::string, std::string>(".pl", "/usr/bin/perl"));
	_cgi.insert(std::pair<std::string, std::string>(".php", "/usr/bin/php"));
}

HttpResponse::~HttpResponse(void) {
	if (_exec_argv)
		free(_exec_argv);
}

std::string	HttpResponse::getHttpResponse() {

	if (_conf.getCanGet() && _request["method"] == "GET")
		methodGet();
	else if (_conf.getCanPost() && _request["method"] == "POST")
		methodPost();
	else if (_conf.getCanDelete() && _request["method"] == "DELETE")
		methodDelete();
	else
		statusRet("405");
	// std::cout << std::endl << "ret: " << std::endl << _ret.c_str() << std::endl; 
	return _ret;
}

void	HttpResponse::methodGet() {
	try {
		_request["pageNoParam"] = _request["fullpage"].substr(0, _request["fullpage"].find("?"));
		std::ifstream page(pageNoParam.c_str());
		if (page) {
			if (is_cgi() == 0)
				setPage("200", page);
		}
		else {
			//Put the variable default_404 in ifstream page() to get the good one from the conf file
			std::ifstream	page("./www/index/index_404.html");
			if (page) {
				setPage("404", page);
			} else
			statusRet("404"); 
		}
	}
	catch (std::exception &e) {
			if (*(_request["fullpage"].end() - 1) != '/')
				statusRet("301");
			else if (_conf.getAutoindex())
				autoIndex();
			else
				statusRet("403");
	}
}

void	HttpResponse::methodPost() {

	statusRet(_request["posted"]);
}

void	HttpResponse::methodDelete() {

	statusRet(_request["deleted"]);
}

void	HttpResponse::setHeader(std::string statusKey) {
	if (_request["method"] == "POST")
		_ret = "HTTP/1.1 " + statusKey + " " + _status[statusKey] + "\r\n" 
		+ "Location: " + "http://" + _request["Host"] + _request["page"] + "\r\n\r\n";
	else
		_ret = "HTTP/1.1 " + statusKey + " " + _status[statusKey] + "\r\n\r\n";
}

void	HttpResponse::setCgiHeader(std::string statusKey) {
	if (_request["method"] == "POST")
		_ret = "HTTP/1.1 " + statusKey + " " + _status[statusKey] + "\r\n"
		+ "Location: " + "http://" + _request["Host"] + _request["page"] + "\r\n";
	else
		_ret = "HTTP/1.1 " + statusKey + " " + _status[statusKey] + "\r\n";
}

void	HttpResponse::setPage(std::string statusKey, std::ifstream &page) {
	size_t pos = std::string::npos;
	std::string	str_page = std::string(
			std::istreambuf_iterator<char>(page),
			std::istreambuf_iterator<char>());
	if ((pos = str_page.find("status: ", 0, 7)) != std::string::npos ||
		(pos = str_page.find("Status: ", 0, 7)) != std::string::npos||
		(pos = str_page.find("STATUS: ", 0, 7)) != std::string::npos)
	{
		while (str_page[pos + 7] == ' ')
			pos++;
		statusKey = str_page.substr(pos + 7, 3);
	}
	if ((pos = str_page.find("\r\n\r\n")) != std::string::npos)
		setCgiHeader(statusKey);
	else
		setHeader(statusKey);
	_ret += str_page;
}

void	HttpResponse::set_exec_argv(std::string cmdPath, std::string errCode) {
	if (errCode == "") {
		_exec_argv = (char **)malloc(sizeof(char *) * 3);
		*(_exec_argv + 2) = (char *)malloc(sizeof(char) * 1);
		*(_exec_argv + 2) = NULL;
		*(_exec_argv + 0) = (char *)cmdPath.c_str();
		*(_exec_argv + 1) = (char *)_request["pageNoParam"].c_str();
	}
	else {
		_exec_argv = (char **)malloc(sizeof(char *) * 5);
		*(_exec_argv + 4) = (char *)malloc(sizeof(char) * 1);
		*(_exec_argv + 4) = NULL;
		*(_exec_argv + 0) = (char *)cmdPath.c_str();
		*(_exec_argv + 1) = (char *)_request["pageNoParam"].c_str();
		*(_exec_argv + 2) = (char *)errCode.c_str();
		*(_exec_argv + 3) = (char *)_status[errCode].c_str();
	}
}

int	HttpResponse::is_cgi() {
	if (_request["fullpage"].find_first_of(".") != std::string::npos)
	{
		if (_request["fullpage"].compare(_request["fullpage"].find_first_of("."), 
				std::string::npos, ".pl", 3) == 0 ||
			_request["fullpage"].compare(_request["fullpage"].find_first_of("."), 
				std::string::npos, ".php", 4) == 0)
		{
			set_exec_argv(_cgi[_request["fullpage"].substr(_request["fullpage"].find_first_of("."))], "");
			setCgiEnv();
			cgi("200");
		}
		else
			return 0;
	}
	else
		return 0;
	return 1;
}

	/***************
	*
	*	if method is get, params should be set to env  
	*	if method is post, params should be sent as stdin
	*	https://en.wikipedia.org/wiki/Common_Gateway_Interface
	*
	***************/

int HttpResponse::cgi(std::string statusKey) {
	int     pipefd[2] = {0, 1};
	//int     pipefdn[2] = {0, 1};

	if (pipe(pipefd) == -1)
		std::cout << "pipe failed" <<std::endl;

	write(pipefd[1], _request.body().c_str(), request.body().Length);
	close(pipefd[1]);


	pid_t   pid = fork();
	std::FILE* tmp = freopen("/tmp/.tmpExecveFd", "wb+", stdout);
	(void)tmp;



	if (pid == -1)
		std::cout << "cgi failed" <<std::endl;
	if (pid == 0) {
		dup2(pipefd[0], 0);
		if (execve(_exec_argv[0], _exec_argv, _env) == -1)
			perror("execve");
	}
	else {
		waitpid(pid, 0, 0);
		close(pipefd[0]);
		std::ifstream tmpst("/tmp/.tmpExecveFd");
		setPage(statusKey, tmpst);
	}
	return 0;
}

std::string	HttpResponse::toUpper(std::string str) {
	std::string::iterator	it;
	for (it = str.begin(); it !=str.end(); ++it)
		*it = std::toupper(*it);
}

void	HttpResponse::setCgiEnv() {
	std::map<std::string, std::string>				env;
	std::string										key;
	std::string										value;
	std::map<std::string, std::string>::iterator	it;

	for (it = _request.begin(); it != _request.end(), ++it)
	{
		key = "HTTP_" + toUpper(it->first);
		std::replace(key.begin(), key.end(), '-', '_');
		env[key] = *it;
	}
	env["CONTENT_LENGTH"] = _request["Content-Length"];
	env["QUERY_STRING"] = _request["page"].substr(_request["page"].find("?") + 1);;
	env["PATH_INFO"] = _request["pageNoParam"];
	env["CONTENT_TYPE"] = _request["content-type"];
	env["REQUEST_METHOD"] = _request["method"];
	env["SCRIPT_NAME"] = _request["page"].substr(0, _request["page"].find("?"));
	env["SERVER_NAME"] = "webserv";
	env["SERVER_PORT"] = _request["Port"];
	env["SERVER_PROTOCOL"] = "HTTP/1.1";
	env["SERVER_SOFTWARE"] = "webserv/1.0";
	env["GATEWAY_INTERFACE"] = "CGI/1.1";
}

void	HttpResponse::statusRet(std::string errCode) {
	std::stringstream output;
	setHeader(errCode);
	output	<< "<!DOCTYPE html>\n"
			<< "<html>\n"
			<< "<body>\n"
			<< "<h1>STATUS "
			<< errCode
			<< "</h1>\n"
			<< "<p>"
			<< _status[errCode]
			<< ".</p>\n"
			<< "</body>\n"
			<< "</html>\n";
	_ret += output.str();
}

void	HttpResponse::autoIndex() {
	DIR	*dp;
	struct  dirent *ep;
	dp = opendir(_request["fullpage"].c_str());
	std::stringstream output;

	setHeader("200");
	output	<< "<!DOCTYPE html>\n"
			<< "<html>\n" 
			<< "\t<head>\n"
			<< "\t\t<meta charset=\"utf-8\" />\n"
			<< "\t\t<link rel=\"stylesheet\" href=\"style.css\" />\n"
			<< "\t\t<title>Melval Kingdom</title>\n"
			<< "\t</head>\n"
			<< "<body>\n";

	if (dp != NULL)
	{
		while ((ep = readdir(dp)))
		{
			output	<< "<p><a href=\""
					<< ep->d_name;
			if (ep->d_type == DT_DIR)
				output << "/";
			std::string tmp(ep->d_name);
				output << "\">"
					<< ep->d_name
					<< "</a></p>\n";
		}
		output << "</body>\n</html>";
		_ret += output.str();
		closedir(dp);
	}
}

void	HttpResponse::initStatus() {
	_status.insert(std::pair<std::string, std::string>("100", "Continue"));
	_status.insert(std::pair<std::string, std::string>("101", "Switching Protocols"));
	_status.insert(std::pair<std::string, std::string>("200", "OK"));
	_status.insert(std::pair<std::string, std::string>("201", "Created"));
	_status.insert(std::pair<std::string, std::string>("202", "Accepted"));
	_status.insert(std::pair<std::string, std::string>("203", "Non-Authoritative Information"));
	_status.insert(std::pair<std::string, std::string>("204", "No Content"));
	_status.insert(std::pair<std::string, std::string>("205", "Reset Content"));
	_status.insert(std::pair<std::string, std::string>("206", "Partial Content"));
	_status.insert(std::pair<std::string, std::string>("300", "Multiple Choices"));
	_status.insert(std::pair<std::string, std::string>("301", "Moved Permanently"));
	_status.insert(std::pair<std::string, std::string>("302", "Moved Temporarily"));
	_status.insert(std::pair<std::string, std::string>("303", "See Other"));
	_status.insert(std::pair<std::string, std::string>("304", "Not Modified"));
	_status.insert(std::pair<std::string, std::string>("305", "Use Proxy"));
	_status.insert(std::pair<std::string, std::string>("400", "Bad Request"));
	_status.insert(std::pair<std::string, std::string>("401", "Unauthorized"));
	_status.insert(std::pair<std::string, std::string>("402", "Payment Required"));
	_status.insert(std::pair<std::string, std::string>("403", "Forbidden"));
	_status.insert(std::pair<std::string, std::string>("404", "Not Found"));
	_status.insert(std::pair<std::string, std::string>("405", "Method Not Allowed"));
	_status.insert(std::pair<std::string, std::string>("406", "Not Acceptable"));
	_status.insert(std::pair<std::string, std::string>("407", "Proxy Authentication Required"));
	_status.insert(std::pair<std::string, std::string>("408", "Request Time-out"));
	_status.insert(std::pair<std::string, std::string>("409", "Conflict"));
	_status.insert(std::pair<std::string, std::string>("410", "Gone"));
	_status.insert(std::pair<std::string, std::string>("411", "Length Required"));
	_status.insert(std::pair<std::string, std::string>("412", "Precondition Failed"));
	_status.insert(std::pair<std::string, std::string>("413", "Request Entity Too Large"));
	_status.insert(std::pair<std::string, std::string>("414", "Request-URI Too Large"));
	_status.insert(std::pair<std::string, std::string>("415", "Unsupported Media Type"));
	_status.insert(std::pair<std::string, std::string>("424", "Failed Dependency"));
	_status.insert(std::pair<std::string, std::string>("500", "Internal Server Error"));
	_status.insert(std::pair<std::string, std::string>("501", "Not Implemented"));
	_status.insert(std::pair<std::string, std::string>("502", "Bad Gateway"));
	_status.insert(std::pair<std::string, std::string>("503", "Service Unavailable"));
	_status.insert(std::pair<std::string, std::string>("504", "Gateway Time-out"));
	_status.insert(std::pair<std::string, std::string>("505", "HTTP Version not supported"));
}

// void	HttpResponse::initCgi() {

// }