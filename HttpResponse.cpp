#include "HttpResponse.hpp"

HttpResponse::HttpResponse(void) : _ret("HTTP/1.1") {
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
	return *this;
}

HttpResponse::~HttpResponse(void) {
}

std::string	HttpResponse::getHttpResponse(std::string path) {
	try {
        std::ifstream	file(path.c_str());
        if (file) {
            
            if (strncmp(path.c_str(), "cgi-bin", 7) == 0) {
                std::cout << "CGI condition" << std::endl;
               cgi(path);
            } else {
                std::string	str_file = std::string(
                        std::istreambuf_iterator<char>(file),
                        std::istreambuf_iterator<char>());
                _ret += " 200 " + _status["200"] + "\n\n";
                _ret += str_file;
            }
            return _ret;
        }
    }
    catch (std::exception &e) {
        if ((strcmp(e.what(), "basic_filebuf::underflow error reading the file: Is a directory") != 0))
            throw e.what();
    }
	return _404NotFound();
}

int HttpResponse::cgi(std::string path) {
    int     pipefd[2] = {0, 1};
    pid_t   pid = fork();
    char    s1[] = "\0";
    // char    s2[] = path.c_str();
    std::string address("/mnt/nfs/home/shlu/42/webserv/");
    address += path;
    char const *argv[] = {address.c_str(), s1};

    std::cout << "is file, path = " << address  << std::endl;
    if (pipe(pipefd) == -1)
        std::cout << "pipe failed" <<std::endl;
    if (pid == -1)
        std::cout << "cgi failed" <<std::endl;
    if (pid == 0) {
        // if method is get, params should be set to env  
        // if method is post, params should be sent as stdin
        // https://en.wikipedia.org/wiki/Common_Gateway_Interface
        if (execve(address.c_str(), (char *const *)argv, NULL) == -1)
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