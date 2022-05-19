#ifndef CLIENTMANAGER_HPP
#define CLIENTMANAGER_HPP

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <unistd.h>
#include <vector>
#include <map>
#include <errno.h>
#include <algorithm>

#include "../config/BlockConfig.hpp"
#include "../http/HttpRequest.hpp"

class ClientManager {
	public:

		ClientManager();
		~ClientManager();
		ClientManager(int fd, const BlockConfig &block);

		void		appendRead(char *buf);
		bool		isReadOk();

		void		setFd(int fd);
		void		setHeaderOk(bool ok);
		void		setReadOk(bool ok);
		void		setSendOk(bool ok);
		void		setRead(std::string str);
		void		setSend(std::string str);

		int				getFd() const;
		bool			getHeaderOk() const;
		bool			getReadOk() const;
		bool			getSendOk() const;
		std::string		getRead() const;
		std::string		getSend() const;
		BlockConfig		getBlock() const;

	private:

		int				_fd;
		bool			_header_ok;
		bool			_read_ok;
		bool			_send_ok;
		std::string		_read;
		std::string		_send;
		BlockConfig		_block;
};

#endif
