#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <iostream>
#include <vector>

class Location {
	public:
		Location(const std::string &arg);
		~Location();

		std::string					getArg() const;
		std::vector<std::string>	getPath() const;
		void						addPath(const std::string &path);

	private:
		Location();
		std::string					_arg;
		std::vector<std::string>	_path;
};

#endif
