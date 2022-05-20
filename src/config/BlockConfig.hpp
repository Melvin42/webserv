#ifndef BLOCKCONFIG_HPP
#define BLOCKCONFIG_HPP

#include <iostream>
#include <fstream>
#include <inttypes.h>
#include <algorithm>
#include <vector>
#include <unistd.h>
#include "Location.hpp"

class BlockConfig {
	public:
		BlockConfig();
		BlockConfig(const BlockConfig &cp);
		~BlockConfig();

		std::string					getHost() const;
		int							getPort() const;
		std::string					getServerName() const;
		std::string					getRoot() const;
		std::string					getCgiRoot() const;
		std::string					getCgiArg() const;
		std::vector<std::string>	getIndex() const;
		std::vector<Location>		getLocation() const;
		std::string					getDefaultIndex() const;
		std::string					getDefault404() const;
		std::string					getToRedirect() const;
		std::string					getRedirectTo() const;
		bool						getCanPost() const;
		bool						getCanGet() const;
		bool						getCanDelete() const;
		bool						getAutoindex() const;
		bool						getIsDefault() const;
		size_t						getBodySizeMax() const;
		size_t						getId() const;

		void	setDefaultIndex();
		void	setDefault404(const std::string &page);
		void	setToRedirect(const std::string &path);
		void	setRedirectTo(const std::string &path);
		void	setNewHost(const std::string &host);
		void	setNewPort(int port);
		void	setNewServerName(const std::string &name);
		void	setNewRoot(const std::string &root);
		void	setNewCgiRoot(const std::string &root);
		void	setNewCgiArg(const std::string &arg);
		void	setNewIndex(const std::string &index);
		void	setNewLocation(const std::string &arg);
		void	addIndexToLocation(const std::string &index, int i);
		void	addRootToLocation(const std::string &root, int i);
		void	addTypeToLocation(const std::string &type, int i);
		void	addCgiToLocationMap(const std::string &key, const std::string &value, int loc_id);
		void	setCanPost(const bool &can_post);
		void	setCanGet(const bool &can_get);
		void	setCanDelete(const bool &can_delete);
		void	setAutoindex(const bool &autoindex);
		void	setIsDefault(const bool &is_default);
		void	setBodySizeMax(const size_t &size_max);
		void	setId(const size_t &id);

		BlockConfig &operator=(const BlockConfig &block);

	private:
		std::string					_default_index;
		std::string					_default_404;
		std::string					_to_redirect;
		std::string					_redirect_to;
		std::string					_host;
		int							_port;
		std::string					_server_name;
		std::string					_root;
		std::string					_cgiRoot;
		std::string					_cgiArg;
		std::vector<std::string>	_index;
		std::vector<Location>		_location;
		bool						_can_post;
		bool						_can_get;
		bool						_can_delete;
		bool						_autoindex;
		bool						_is_default;
		size_t						_body_size_max;
		size_t						_id;
};

#endif
