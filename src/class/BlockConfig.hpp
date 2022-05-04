#ifndef BLOCKCONFIG_HPP
#define BLOCKCONFIG_HPP

#include <iostream>
#include <fstream>
#include <inttypes.h>
#include <algorithm>
#include <vector>

class BlockConfig {
	public:
		BlockConfig();
		~BlockConfig();

		std::vector<int>			getPort() const;
		std::vector<std::string>	getServerName() const;
		std::vector<std::string>	getRoot() const;
		std::vector<std::string>	getLocation() const;

		void						setNewPort(int port);
		void						setNewServerName(std::string name);
		void						setNewRoot(std::string root);
		void						setNewLocation(std::string loc);

	private:
		std::vector<int>			_port;
		std::vector<std::string>	_server_name;
		std::vector<std::string>	_root;
		std::vector<std::string>	_location;
};

#endif
