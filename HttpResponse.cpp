#include "HttpResponse.hpp"

HttpResponse::HttpResponse(void) : _ret("HTTP/1.1"), _env(NULL) {

}

HttpResponse::HttpResponse(char **env) : _ret("HTTP/1.1"), _env(env) {
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
}

HttpResponse::HttpResponse(const HttpResponse &httpresponse) {
	*this = httpresponse;
}

HttpResponse &HttpResponse::operator=(const HttpResponse &httpresponse) {
	_ret = httpresponse._ret;
	_status = httpresponse._status;
    _env = httpresponse._env;
	return *this;
}

HttpResponse::~HttpResponse(void) {
}

std::string	HttpResponse::getHttpResponse(std::string requestedPagePath) {
	try {
        std::ifstream	page(requestedPagePath.c_str());
        if (page) {
            if (cgi_parcing_check(requestedPagePath) == 1)
                getPage(page, "200");
            return _ret;
        }
    }
    catch (std::exception &e) {
        if ((strcmp(e.what(), "basic_filebuf::underflow error reading the file: Is a directory") != 0))
            throw e.what();
    }
	return _404NotFound();
}

void        HttpResponse::getPage(std::ifstream	&page, std::string statusKey) {
    std::string	str_page = std::string(
            std::istreambuf_iterator<char>(page),
            std::istreambuf_iterator<char>());
    _ret += " " + statusKey + " " + _status[statusKey] + "\n\n";
    _ret += str_page;
}

int    HttpResponse::cgi_parcing_check(std::string requestedPagePath) {
    if (requestedPagePath.find_first_of(".") != std::string::npos)
    {
        if (requestedPagePath.compare(requestedPagePath.find_first_of("."), std::string::npos, ".pl", 3) == 0)
            cgi(get_execve_argv(requestedPagePath, "perl"));
        else if (requestedPagePath.compare(requestedPagePath.find_first_of("."), std::string::npos, ".php", 4) == 0)
            cgi(get_execve_argv(requestedPagePath, "php"));
        else
            return 1;
    }
    else
        return 1;
    return 0;
}

std::string HttpResponse::get_cmd_bin(std::string cmd) const {
    struct dirent	*ent;
	struct stat		statbuf;
    char    *path = getenv("PATH");
    char    *token = strtok(path, ":");
    ret = NULL;
	ent = readdir(dir);
	while (ent)
	{
        while (token)
        {

        }
    }
    std::string bin(cmd);

    return bin;
}

char    **HttpResponse::get_execve_argv
        (std::string requestedPagePath, std::string cmd) const {
char **argv;
    std::string cmd_bin = get_cmd_bin(cmd);
    std::string full_page_path(getenv("PWD"));
    full_page_path += "/" + requestedPagePath;
    if ((argv = (char **)malloc(sizeof(char *) * 3)) == NULL)
        return NULL;
    *argv = (char *)cmd_bin.c_str();
    *(argv + 1) = (char *)full_page_path.c_str();
    *(argv + 2) = NULL;
std::cout << "*argv: " << *argv << std::endl;
std::cout << "*argv + 1: " << *(argv + 1) << std::endl;
std::cout << "*argv + 2: " << *(argv + 2) << std::endl;
    return argv;
}

int HttpResponse::cgi(char **execve_argv) {

    int     pipefd[2] = {0, 1};
    pid_t   pid = fork();
    
    std::cout << "bin_cmd = " << *execve_argv  << std::endl;
    std::cout << "full_page_path = " << *(execve_argv + 1)  << std::endl;
    if (pipe(pipefd) == -1)
        std::cout << "pipe failed" <<std::endl;
    if (pid == -1)
        std::cout << "cgi failed" <<std::endl;
    if (pid == 0) {
        // if method is get, params should be set to env  
        // if method is post, params should be sent as stdin
        // https://en.wikipedia.org/wiki/Common_Gateway_Interface
        if (execve(*execve_argv, execve_argv + 1, NULL) == -1)
           // std::cout << "execve failed" <<std::endl;
            perror("execve");
    }
    else {
        waitpid(pid, 0,0);
        close(pipefd[0]);
        close(pipefd[1]);
    }
    return 0;
}

// std::string HttpResponse::interface(std::string errorCode) {

// }

std::string HttpResponse::_404NotFound(void) {
    std::ifstream	not_found_file("webpages/not_found.html");
	if (!not_found_file) {
		_ret += " 500 " + _status["500"] + "\n\n";
		return _ret;
	}
	_ret += " 404 " + _status["404"] + "\n\n";
	std::string	str_not_found = std::string(
			std::istreambuf_iterator<char>(not_found_file),
			std::istreambuf_iterator<char>());
	_ret += str_not_found;
    return _ret;
}