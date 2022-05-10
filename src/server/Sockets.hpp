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

#include "ClientManager.hpp"
#include "../http/HttpRequest.hpp"
#include "../http/HttpResponse.hpp"

class Socket {
	public:

//		Socket(const Socket &socket);
		virtual ~Socket();

		std::string			receiveLine();
		void				closeFd();
		int					getMasterFd() const;
		char				**getEnv();
		std::vector<ClientManager>	&getClientSocket();
//		std::vector<int>	&getClientSocket();


//		Socket &operator=(const Socket &socket);

	protected:
		friend class SocketServer;

		Socket();

		int					_server_fd;
		char				**_env;
		std::vector<ClientManager>	_clientSocket;
//		std::vector<int>	_clientSocket;
		struct sockaddr_in	_address;

	private:
};

class SocketServer : public Socket {
	public:
//		SocketServer(int port, int connections);
		SocketServer(char **env, int port, int connections);

		int		getSocketUsed() const;
		void	setSocketUsed(int fd);
		fd_set	getReadFds() const;
		fd_set	getWriteFds() const;

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
		fd_set	_writefds;
};

#endif
