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
#include <exception>

#include "ClientManager.hpp"
#include "../config/Config.hpp"
#include "../http/HttpRequest.hpp"
#include "../http/HttpResponse.hpp"

#include <string.h>

extern bool	g_sigbool;

class Socket {
	public:

		Socket();
		virtual ~Socket();

		std::string					receiveLine();
		void						closeFd();
		int							getMasterFd(size_t id) const;
		char						**getEnv();
		std::vector<ClientManager>	&getClientSocket();
		void						addServerFd(int fd);

	protected:
		friend class SocketServer;

		std::vector<int>			_server_fd;
		std::vector<ClientManager>	_clientSocket;
		struct sockaddr_in			_address;

	private:
};

class SocketServer : public Socket {
	public:
		SocketServer(const Config &conf);

		int		getSocketUsed() const;
		void	setSocketUsed(int fd);
		fd_set	getReadFds() const;
		fd_set	getWriteFds() const;

		bool	setUpBlockServer();
		bool	bindSocket(const BlockConfig &block);
		int		acceptSocket(const BlockConfig &block);
		void	FdZero();
		void	setFdSet(const BlockConfig &block);
		void	selectSocket();
		bool	ready(int fd, fd_set set);
		void	setClientSocket(const BlockConfig &block);
		void	simultaneousRead();
		void	simultaneousWrite();
		void	handleClient();
		void	run();
		void	closeClean();

		class	SelectException : public std::exception {
			const char	*what(void) const throw();
		};

		class	InvalidSocketException : public std::exception {
			const char	*what(void) const throw();
		};

	private:
		int		_sd;
		int		_max_sd;
		fd_set	_readfds;
		fd_set	_writefds;
		Config	_config;
};

#endif
