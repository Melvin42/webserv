#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <iostream>
#include <vector>
#include <map>

class Location {
	public:
		Location(const std::string &arg);
		Location(const Location &cp);
		~Location();

		std::string							getArg() const;
		std::vector<std::string>			getIndex() const;
		std::map<std::string, std::string>	getCgiMap() const;

		void								addIndex(const std::string &index);
		void								addCgiPath(const std::string &key,
													const std::string &value);

		void	printCgiMap();

		Location &operator=(const Location &loc);

	private:
		Location();
		std::string							_arg;
		std::string							_type;
		std::vector<std::string>			_index;
		std::map<std::string, std::string>	_cgi;
};

#endif
