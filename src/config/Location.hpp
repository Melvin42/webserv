#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <iostream>
#include <vector>

class Location {
	public:
		Location(const std::string &arg);
		~Location();

		std::string					getArg() const;
		std::vector<std::string>	getIndex() const;
		std::vector<std::string>	getCgiBinary() const;
		std::vector<std::string>	getCgiFilename() const;
		int							getCgiBinaryNbr() const;
		int							getCgiFilenameNbr() const;

		void						incCgiBinaryNbr();
		void						incCgiFilenameNbr();
		void						addIndex(const std::string &index);
		void						addCgiBinary(const std::string &cgi_binary);
		void						addCgiFilename(const std::string &cgi_filename);

	private:
		Location();
		std::string					_arg;
		std::string					_type;
		int							_cgi_binary_nbr;
		int							_cgi_filename_nbr;
		std::vector<std::string>	_index;
		std::vector<std::string>	_cgi_binary;
		std::vector<std::string>	_cgi_filename;
};

#endif
