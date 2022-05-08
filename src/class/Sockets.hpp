#ifndef SOCKETS_HPP
#define SOCKETS_HPP

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <unistd.h>
#include <vector>
#include <errno.h>
#include <algorithm>

#include "define.h"
#include "ClientManager.hpp"
#include "HttpResponse.hpp"
#include "HttpRequest.hpp"

class Socket {
	public:

//		Socket(const Socket &socket);
		virtual ~Socket();

		std::string			receiveLine();
		void				closeFd();
		int					getMasterFd() const;
		std::vector<ClientManager>	&getClientSocket();
//		std::vector<int>	&getClientSocket();


		Socket &operator=(const Socket &socket);

	protected:
		friend class SocketServer;

		Socket();

		int					_server_fd;
		std::vector<ClientManager>	_clientSocket;
//		std::vector<int>	_clientSocket;
		struct sockaddr_in	_address;

	private:
};

class SocketServer : public Socket {
	public:
		SocketServer(int port, int connections);

		int		getSocketUsed() const;
		void	setSocketUsed(int fd);
		fd_set	getReadFds() const;

		int		acceptSocket();
		void	selectSocket();
		bool	ready(int fd, fd_set set);
		void	setClientSocket();
		void	simultaneousRead();
		void	run();
		void	closeClean();

	private:
		int		_sd;
		int		_max_sd;
		fd_set	_readfds;
};

#endif
