#ifndef BLOCKCONFIG_HPP
#define BLOCKCONFIG_HPP

#include <iostream>
#include <fstream>
#include <inttypes.h>
#include <algorithm>
#include <vector>
#include "Location.hpp"

class BlockConfig {
	public:
		BlockConfig();
		~BlockConfig();

		std::vector<int>			getPort() const;
		std::vector<std::string>	getServerName() const;
		std::vector<std::string>	getRoot() const;
		std::vector<std::string>	getIndex() const;
		std::vector<Location>		getLocation() const;

		void						setNewPort(int port);
		void						setNewServerName(const std::string &name);
		void	setNewRoot(const std::string &root);
		void	setNewIndex(const std::string &index);
		void	setNewLocation(const std::string &arg);
		void	addPathToLocation(const std::string &path, int index);

	private:
		std::vector<int>			_port;
		std::vector<std::string>	_server_name;
		std::vector<std::string>	_root;
		std::vector<std::string>	_index;
		std::vector<Location>		_location;
};

#endif
