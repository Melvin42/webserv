#include "HttpResponse.hpp"

HttpResponse::HttpResponse(void) : _ret("HTTP/1.1"), _root(""), _full_path(""), _env(NULL), _exec_argv(NULL) {
}

HttpResponse::HttpResponse(char **env) : _ret("HTTP/1.1"), _full_path(""), _env(env) {
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
	_exec_argv = (char **)malloc(sizeof(char *) * 3);
	*(_exec_argv + 2) = (char *)malloc(sizeof(char) * 1);
	*(_exec_argv + 2) = NULL;
}

HttpResponse::~HttpResponse(void) {
	free(_exec_argv);
}

void	HttpResponse::getHeader(std::string statusKey) {
	_ret += " " + statusKey + " " + _status[statusKey] + "\n\n";
}

void	HttpResponse::getPage(std::ifstream	&page) {
	std::string	str_page = std::string(
			std::istreambuf_iterator<char>(page),
			std::istreambuf_iterator<char>());
	_ret += str_page;
}

std::string	HttpResponse::getHttpResponse(std::string requestedPagePath) {
	try {
		std::ifstream	page(requestedPagePath.c_str());
		if (page) {
			if (is_cgi(requestedPagePath) == 0)
			{
				getHeader("200");
				getPage(page);
			}
			return _ret;
		}
	}
	catch (std::exception &e) {
		if ((strcmp(e.what(), 
						"basic_filebuf::underflow error reading the file: Is a directory") != 0))
			throw e.what();
	}
	return _404NotFound();
}

void	HttpResponse::set_exec_argv(std::string requestedPagePath,
		std::string cmdPath) {
	_full_path += _root + "/" + requestedPagePath; //i changed some stuff it can be undone
	*_exec_argv = (char *)cmdPath.c_str();
	*(_exec_argv + 1) = (char *)_full_path.c_str();
}

int	HttpResponse::is_cgi(std::string requestedPagePath) {
	if (requestedPagePath.find_first_of(".") != std::string::npos)
	{
		if (requestedPagePath.compare(requestedPagePath.find_first_of("."), 
					std::string::npos, ".pl", 3) == 0 ||
				requestedPagePath.compare(requestedPagePath.find_first_of("."), 
					std::string::npos, ".php", 4) == 0)
		{
			set_exec_argv(requestedPagePath, 
					_cgi[requestedPagePath.substr(requestedPagePath.find_first_of("."))]);
			cgi();
		}
		else
			return 0;
	}
	else
		return 0;
	return 1;
}

int HttpResponse::cgi() {
	int     pipefd[2] = {0, 1};
	pid_t   pid = fork();
	std::FILE* tmp = freopen("tmp", "wb+", stdout);

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
		st += "/tmp";
		std::ifstream tmpst(st.c_str());
		getPage(tmpst);
		(void)tmp;
	}
	return 0;
}

// std::string HttpResponse::interface(std::string errorCode) {

// }

std::string HttpResponse::_404NotFound(void) {
	std::ifstream	not_found_page("webpages/not_found.html");
	if (!not_found_page)
		getHeader("500");
	else {
		getHeader("404");
		getPage(not_found_page);
	}
	return _ret;
}
