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

		int							getPort() const;
		std::string					getServerName() const;
		std::string					getRoot() const;
		std::vector<std::string>	getIndex() const;
		std::vector<Location>		getLocation() const;

		void	setNewPort(int port);
		void	setNewServerName(const std::string &name);
		void	setNewRoot(const std::string &root);
		void	setNewIndex(const std::string &index);
		void	setNewLocation(const std::string &arg);
		void	addIndexToLocation(const std::string &index, int i);
		void	addCgiToLocationMap(const std::string &key, const std::string &value, int loc_id);

	private:
		int												_port;
		std::string										_server_name;
		std::string										_root;
		std::vector<std::string>						_index;
		std::vector<Location>							_location;
//		bool											_can_post;
//		bool											_autoindex;
//		size_t											_body_size_max;
};

#endif
