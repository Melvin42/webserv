#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <ctype.h>
#include <limits>
#include <stdlib.h>
#include <sys/stat.h>
#include "../server/ClientManager.hpp"
#include "../config/Config.hpp"

class	HttpRequest {

	public:

		HttpRequest(const char *buffer, const BlockConfig &conf);
		HttpRequest(const HttpRequest &httprequest);
		~HttpRequest();
		HttpRequest &operator=(const HttpRequest &httprequest);

		std::string							getMethod();
		std::string							getFullPage();
		std::string							getVersion();
		std::string							getHost();
		size_t								getContentLength();
		std::map<std::string, std::string>	getRequest() const;

		void								postCheck(std::stringstream &line);
		void								parseHeader(std::stringstream &line);
		void								parseBody(std::stringstream &line);

	private:

		HttpRequest();
		void								setFullPage();
		std::string							getFilename(std::map<std::string, std::string>	&bodyHeader);
		std::string							getKey(std::string buf);
		std::string							getValue(std::string buf);
		bool								fileIssue(std::string filename);

		std::map<std::string, std::string>	_request;
		BlockConfig 						_conf;
};

#endif
