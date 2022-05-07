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

#include "define.h"

class ClientManager {
	public:

		ClientManager();
		~ClientManager();
		ClientManager(int fd);

		void		appendRead(char *buf);
		bool		isReadOk();()

		void		setFd(int fd);
		void		setReadOk(bool ok);
		void		setSendOk(bool ok);
		void		setRead(str::string str);
		void		setSend(str::string str);

		int				getFd();
		bool			getReadOk();
		bool			getSendOk();
		std::string		getRead();
		std::string		getSend();

	private:

		int				_fd;
		bool			_read_ok;
		bool			_send_ok;
		std::string		_read;
		std::string		_send;
}

#endif
