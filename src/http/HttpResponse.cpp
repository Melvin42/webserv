#include "HttpResponse.hpp"

#include <string>

HttpResponse::HttpResponse() {
}

HttpResponse::HttpResponse(BlockConfig config, std::map<std::string, std::string> request)
 : _env(NULL), _exec_argv(NULL), _conf(config), _ret(""), _request(request) {

	initStatus();
	initCgi();
}

HttpResponse::~HttpResponse(void) {
	if (_exec_argv)
	{
		for (int i = 0; *(_exec_argv + i) != NULL; i++)
			free(*(_exec_argv + i));
		free(_exec_argv);
	}
}

std::string	HttpResponse::getHttpResponse() {

	if (_conf.getCanGet() && _request["method"] == "GET")
		methodGet();
	else if (_conf.getCanPost() && _request["method"] == "POST" )
		methodPost();
	else if (_conf.getCanDelete() && _request["method"] == "DELETE")
		methodDelete();
	else
		statusRet("405");
	return _ret;
}

void	HttpResponse::methodGet() {
	try {
		_request["pageNoParam"] = _request["fullpage"].substr(0, _request["fullpage"].find("?"));
		std::ifstream page(_request["pageNoParam"].c_str());
		if (page) {
			if (is_cgi() == 0)
				setPage("200", page);
		}
		else if (*(_request["pageNoParam"].end()) == '/') {
			if (_conf.getAutoindex())
			{
				_request["fullpage"] = _conf.getRoot() + _request["page"];
				autoIndex();
			}
			else
				statusRet("403");
		}
		else {
			//Put the variable default_404 in ifstream page() to get the good one from the conf file
			std::ifstream	page(_conf.getDefault404().c_str());
			if (page) {
				setPage("404", page);
			} else
			statusRet("404");
		}
	}
	catch (std::exception &e) {
		std::cerr<< e.what() << std::endl;
			if (*(_request["page"].end() - 1) != '/' && *(_request["page"].end() - 1) != '?')
				statusRet("301");
			else if (_conf.getAutoindex())
				autoIndex();
			else
				statusRet("403");
	}
}

void	HttpResponse::methodPost() {

	if (_request.find("posted") != _request.end())
		statusRet(_request["posted"]);
	else 
		methodGet();
}

void	HttpResponse::methodDelete() {

	statusRet(_request["deleted"]);
}

void	HttpResponse::setHeader(std::string statusKey) {
	std::cerr << "(HttpResponse.cpp:88) " << _request["page"] << std::endl;
	if (statusKey == "301" && *(_request["page"].end() - 1) != '?')
		_ret = "HTTP/1.1 " + statusKey + " " + _status[statusKey] + "\r\n" 
		+ "Location: " + _request["page"].substr(_request["page"].find("/"), _request["page"].size()) + "/" + "\r\n\r\n";
	else if (_request["method"] == "POST")
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

void	HttpResponse::set_exec_argv(std::string cmdPath) {
	_exec_argv = (char **)malloc(sizeof(char *) * 3);
	*(_exec_argv + 2) = (char *)malloc(sizeof(char) * 1);
	*(_exec_argv + 2) = NULL;
	*(_exec_argv + 0) = (char *)strdup(cmdPath.c_str());
	*(_exec_argv + 1) = (char *)strdup(_request["pageNoParam"].c_str());
}

bool	HttpResponse::findCgi() {

	std::map<std::string, std::string>::iterator it = _cgi.begin();
	std::map<std::string, std::string>::iterator ite = _cgi.end();
	for (; it != ite; ++it)
	{
		if (_request["pageNoParam"].compare(_request["pageNoParam"].find_first_of("."), 
			std::string::npos, it->first, it->first.size()) == 0)
			return true;
	}
	return false;
}

int	HttpResponse::is_cgi() {
	if (_request["fullpage"].find_first_of(".") != std::string::npos)
	{
		if (findCgi())
		{
			set_exec_argv(_cgi[_request["pageNoParam"].substr(_request["pageNoParam"].find_first_of("."))]);
			getEnv();
			cgi("200");
		}
		else
			return 0;
	}
	else
		return 0;
	return 1;
}

int HttpResponse::cgi(std::string statusKey) {

	std::ofstream ExecIn("/tmp/.ExecIn");
	ExecIn << _request["body"];
	ExecIn.close();
	std::FILE* tmpIn = freopen("/tmp/.ExecIn", "rb+", stdin);
	(void)tmpIn;
	pid_t   pid = fork();
	if (pid == -1)
	{
		std::cout << "cgi failed" <<std::endl;
		return 1;
	}
	if (pid == 0) {

		std::FILE* tmpOut = freopen("/tmp/.ExecOut", "wb+", stdout);
		(void)tmpOut;
		if (execve(_exec_argv[0], _exec_argv, _env) == -1)
			perror("execve");
	}
	else {
		waitpid(pid, 0, 0);
		std::ifstream ExecOut("/tmp/.ExecOut");
		setPage(statusKey, ExecOut);
	}
	return 0;
}

void		HttpResponse::getEnv() {
	int i = 0;
	std::map<std::string, std::string> env = initEnv();
	std::map<std::string, std::string>::iterator	it;
	_env = (char **)malloc(sizeof(char *) * (env.size() + 1));
	*(_env + env.size()) = (char *)malloc(sizeof(char) * 1);
			*(_env + env.size()) = NULL;
	for (it = env.begin(); it != env.end(); ++it, i++)
	{
		std::string	str = it->first + "=" + it->second;
		*(_env + i) = (char *)malloc(sizeof(char *) * (str.size() + 1));
		strcpy(*(_env + i), str.c_str());
	}
}

std::map<std::string, std::string>	HttpResponse::initEnv() {
	std::map<std::string, std::string>				env;
	std::string										key;
	std::string										value;
	std::map<std::string, std::string>::iterator	it;

	for (it = _request.begin(); it != _request.end(); ++it)
	{
		if (it->first == "body" || it->first == "method" 
			|| it->first == "fullpage" || it->first == "pageNoParam")
			continue ;
		key = "HTTP_" + toUpper(it->first);
		std::replace(key.begin(), key.end(), '-', '_');
		env[key] = it->second;
	}
	env["CONTENT_LENGTH"] = _request["content-length"];
		env["CONTENT_TYPE"] = _request["content-type"];
	if (_request["page"].find("?") != std::string::npos)
		env["QUERY_STRING"] = _request["page"].substr(_request["page"].find("?") + 1, _request["page"].size());
	env["PATH_INFO"] = _request["pageNoParam"];
	env["PATH_TRANSLATED"] = _request["page"];
	env["SCRIPT_FILENAME"] = _request["pageNoParam"];
	env["REQUEST_METHOD"] = _request["method"];
	env["SCRIPT_NAME"] = _request["page"].substr(_request["page"].find_last_of("/\\") + 1);
	env["SERVER_NAME"] = _request["host"];
	env["SERVER_PORT"] = _conf.getPort();
	env["SERVER_PROTOCOL"] = "HTTP/1.1";
	env["SERVER_SOFTWARE"] = "webserv/1.0";
	env["GATEWAY_INTERFACE"] = "CGI/1.1";
	env["REQUEST_URI"] = _request["fullpage"];
	env["REDIRECT_STATUS"] = "1";
	return env;
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
	dp = opendir(_request["pageNoParam"].c_str());
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

std::string	HttpResponse::toUpper(std::string str) {
	std::string::iterator	it;

	for (it = str.begin(); it !=str.end(); ++it)
		*it = std::toupper(*it);
	return str;
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

void	HttpResponse::initCgi() {
	for (size_t i = 0; i < _conf.getLocation().size(); i++) {
        if (_conf.getLocation().at(i).getType() == "cgi") {
            _cgi = _conf.getLocation().at(i).getCgiMap();
        }
    }
}
