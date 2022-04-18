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

class Socket {
	public:

		Socket(const Socket &socket);
		virtual ~Socket();

		std::string			receiveLine();
		void				closeFd();
		void				SeindLine(std::string);
		int					getMasterFd();
		std::vector<int>	&getClientSocket();


		Socket &operator=(const Socket &socket);

	protected:
		friend class SocketServer;
//		friend class SocketSelect;

//		Socket(int server_fd);
		Socket();

		int					_server_fd;
		std::vector<int>	_clientSocket;
		struct sockaddr_in	_address;

	private:
		static void	Start();
		static void	End();
		static int	_nofSockets;
};

class SocketClient : public Socket {
	public:
		SocketClient(const std::string &host, int port);
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
		void	closeClean();

	private:
		int		_sd;
		int		_max_sd;
		fd_set	_readfds;
};

#endif
