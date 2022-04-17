#include "HttpResponse.hpp"

HttpResponse::HttpResponse(void) : _ret("HTTP/1.1") {
                    _status.insert({"100", "Continue"});
                    _status.insert({"101", "Switching Protocols"});
                    _status.insert({"200", "OK"});
                    _status.insert({"201", "Created"});
                    _status.insert({"202", "Accepted"});
                    _status.insert({"203", "Non-Authoritative Information"});
                    _status.insert({"204", "No Content"});
                    _status.insert({"205", "Reset Content"});
                    _status.insert({"206", "Partial Content"});
                    _status.insert({"300", "Multiple Choices"});
                    _status.insert({"301", "Moved Permanently"});
                    _status.insert({"302", "Moved Temporarily"});
                    _status.insert({"303", "See Other"});
                    _status.insert({"304", "Not Modified"});
                    _status.insert({"305", "Use Proxy"});
                    _status.insert({"400", "Bad Request"});
                    _status.insert({"401", "Unauthorized"});
                    _status.insert({"402", "Payment Required"});
                    _status.insert({"403", "Forbidden"});
                    _status.insert({"404", "Not Found"});
                    _status.insert({"405", "Method Not Allowed"});
                    _status.insert({"406", "Not Acceptable"});
                    _status.insert({"407", "Proxy Authentication Required"});
                    _status.insert({"408", "Request Time-out"});
                    _status.insert({"409", "Conflict"});
                    _status.insert({"410", "Gone"});
                    _status.insert({"411", "Length Required"});
                    _status.insert({"412", "Precondition Failed"});
                    _status.insert({"413", "Request Entity Too Large"});
                    _status.insert({"414", "Request-URI Too Large"});
                    _status.insert({"415", "Unsupported Media Type"});
                    _status.insert({"500", "Internal Server Error"});
                    _status.insert({"501", "Not Implemented"});
                    _status.insert({"502", "Bad Gateway"});
                    _status.insert({"503", "Service Unavailable"});
                    _status.insert({"504", "Gateway Time-out"});
                    _status.insert({"505", "HTTP Version not supported"});
}

HttpResponse::HttpResponse(const HttpResponse &httpresponse) {
	*this = httpresponse;
	return ;
}

HttpResponse &HttpResponse::operator=(const HttpResponse &httpresponse) {
	_ret = httpresponse._ret;
	_status = httpresponse._status;
	return *this;
}

HttpResponse::~HttpResponse(void) {
}

std::string	HttpResponse::getHttpResponse(std::string path) {
	std::ifstream	file(path);
	if (!file) {
		std::ifstream	not_found_file("not_found.html");
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
	std::string	str_file = std::string(
				std::istreambuf_iterator<char>(file),
				std::istreambuf_iterator<char>());
	_ret += " 200 " + _status["200"] + "\n\n";
	_ret += str_file;
	return _ret;
}
