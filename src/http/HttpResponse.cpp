#include "HttpResponse.hpp"

HttpResponse::HttpResponse(void) : _ret(""), _root(""), _full_path(""), _env(NULL), _exec_argv(NULL) {
}

HttpResponse::HttpResponse(char **env) : _ret(""), _full_path(""), _env(env), _exec_argv(NULL) {
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
	_status.insert(std::pair<std::string, std::string>("500", "Internal Server Error"));
	_status.insert(std::pair<std::string, std::string>("501", "Not Implemented"));
	_status.insert(std::pair<std::string, std::string>("502", "Bad Gateway"));
	_status.insert(std::pair<std::string, std::string>("503", "Service Unavailable"));
	_status.insert(std::pair<std::string, std::string>("504", "Gateway Time-out"));
	_status.insert(std::pair<std::string, std::string>("505", "HTTP Version not supported"));

	//tmply be like this, _cgi should be configured by input .conf file
	_cgi.insert(std::pair<std::string, std::string>(".pl", "/usr/bin/perl"));
	_cgi.insert(std::pair<std::string, std::string>(".php", "/usr/bin/php"));
	_root = getenv("PWD");
}

HttpResponse::~HttpResponse(void) {
	if (_exec_argv)
		free(_exec_argv);
}

void	HttpResponse::getHeader(std::string statusKey) {
	_ret = "HTTP/1.1 " + statusKey + " " + _status[statusKey] + "\r\n\r\n";
}

void	HttpResponse::getPage(std::string statusKey, std::ifstream	&page) {
	std::string	str_page = std::string(
			std::istreambuf_iterator<char>(page),
			std::istreambuf_iterator<char>());
	getHeader(statusKey);
	_ret += str_page;
}

std::string	HttpResponse::getHttpResponse(std::string requestedPagePath) {
	try {
		std::ifstream	page(requestedPagePath.c_str());
		if (page) {
			if (is_cgi(requestedPagePath) == 0)
				getPage("200", page);
		}
		else
			errRet("404");
	}
	catch (std::exception &e) {
//			std::cout << e.what();
			if (*(requestedPagePath.end() - 1) != '/')
				errRet("301");
			else
				autoIndex(requestedPagePath);
			//if autoindex off, 403 forbidden
			// errCgi("403");
	}
	return _ret;
}

void	HttpResponse::set_exec_argv(std::string requestedCgiPath,
		std::string cmdPath, std::string errCode) {
	if (errCode == "") {
		_exec_argv = (char **)malloc(sizeof(char *) * 3);
		*(_exec_argv + 2) = (char *)malloc(sizeof(char) * 1);
		*(_exec_argv + 2) = NULL;
		*(_exec_argv + 0) = (char *)cmdPath.c_str();
		_full_path += _root + "/" + requestedCgiPath;
		*(_exec_argv + 1) = (char *)_full_path.c_str();
	}
	else {
		_exec_argv = (char **)malloc(sizeof(char *) * 5);
		*(_exec_argv + 4) = (char *)malloc(sizeof(char) * 1);
		*(_exec_argv + 4) = NULL;
		*(_exec_argv + 0) = (char *)cmdPath.c_str();
		_full_path += _root + "/" + requestedCgiPath;
		*(_exec_argv + 1) = (char *)_full_path.c_str();
		*(_exec_argv + 2) = (char *)errCode.c_str();
		*(_exec_argv + 3) = (char *)_status[errCode].c_str();
	}
}
	/*
void	HttpResponse::set_exec_argv(std::string requestedPagePath,
		std::string cmdPath) {
	_full_path += _root + "/" + requestedPagePath; //i changed some stuff it can be undone
	*_exec_argv = (char *)cmdPath.c_str();
	*(_exec_argv + 1) = (char *)_full_path.c_str();
}
*/
int	HttpResponse::is_cgi(std::string requestedPagePath) {
	if (requestedPagePath.find_first_of(".") != std::string::npos)
	{
		if (requestedPagePath.compare(requestedPagePath.find_first_of("."), 
					std::string::npos, ".pl", 3) == 0 ||
				requestedPagePath.compare(requestedPagePath.find_first_of("."), 
					std::string::npos, ".php", 4) == 0)
		{
			set_exec_argv(requestedPagePath, 
					_cgi[requestedPagePath.substr(requestedPagePath.find_first_of("."))], "");
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
	int     pipefd[2] = {0, 1};
	pid_t   pid = fork();
	std::FILE* tmp = freopen(".tmpExecveFd", "wb+", stdout);

	if (pipe(pipefd) == -1)
		std::cout << "pipe failed" <<std::endl;
	if (pid == -1)
		std::cout << "cgi failed" <<std::endl;
	if (pid == 0) {
		// if method is get, params should be set to env  
		// if method is post, params should be sent as stdin
		// https://en.wikipedia.org/wiki/Common_Gateway_Interface
		if (execve(_exec_argv[0], _exec_argv, _env) == -1)
			perror("execve");
	}
	else {
		waitpid(pid, 0, 0);
		close(pipefd[0]);
		close(pipefd[1]);
		std::string st = _root;
		st += "/.tmpExecveFd";
		std::ifstream tmpst(st.c_str());
		getPage(statusKey, tmpst);
		(void)tmp;
		remove(".tmpExecveFd");
	}
	return 0;
}

void	HttpResponse::errRet(std::string errCode) {
	std::stringstream output;
	getHeader(errCode);
	if (errCode != "404") {
		output	<< "<!DOCTYPE html>\n"
				<< "<html>\n"
				<< "<body>\n"
				<< "<h1>ERROR "
				<< errCode
				<< "</h1>\n"
				<< "<p>"
				<< _status[errCode]
				<< ".</p>\n"
				<< "</body>\n"
				<< "</html>\n";
		_ret += output.str();
	} else {
		std::ifstream	file("./index/404/index_404.html");

		getPage("404", file);
	}
}

void	HttpResponse::autoIndex(std::string requestedPagePath) {
	DIR	*dp;
	struct  dirent *ep;
	dp = opendir(requestedPagePath.c_str());
	std::stringstream output;

	getHeader("200");
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
			output << "\">"
					<< ep->d_name
					<< "</a></p>\n";
		}
		output << "</body>\n</html>";
		_ret += output.str();
		closedir(dp);
	}
}

// std::string HttpResponse::interface(std::string errorCode) {

// }
