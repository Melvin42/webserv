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

		void		incrementValRead(int valread);
		void		appendRead(char *buf, int valread);
		int			isReadOk();

		void		setFd(int fd);
		void		setHeaderOk(bool ok);
		void		setReadOk(int ok);
		void		setSendOk(bool ok);
		void		setRead(std::string str);
		void		setSend(std::string str);

		int				getFd() const;
		bool			getHeaderOk() const;
		int				getReadOk() const;
		bool			getSendOk() const;
		std::string		getRead() const;
		std::string		getSend() const;
		BlockConfig		getBlock() const;

	private:

		int				_fd;
		bool			_header_ok;
		int				_read_ok;
		bool			_send_ok;
		std::string		_read;
		std::string		_send;
		int				_valRead;
		BlockConfig		_block;
};

#endif
