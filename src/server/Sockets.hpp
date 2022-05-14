#ifndef SOCKETS_HPP
#define SOCKETS_HPP

#define MAXCLIENT 30
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <unistd.h>
#include <vector>
#include <errno.h>
#include <algorithm>

#include "ClientManager.hpp"
#include "../config/Config.hpp"
#include "../http/HttpRequest.hpp"
#include "../http/HttpResponse.hpp"

class Socket {
	public:

		Socket();
		virtual ~Socket();

		std::string					receiveLine();
		void						closeFd();
		int							getMasterFd(size_t i) const;
		char						**getEnv();
		std::vector<ClientManager>	&getClientSocket();


	protected:
		friend class SocketServer;

		std::vector<int>			_server_fd;
		char						**_env;
		std::vector<ClientManager>	_clientSocket;
		struct sockaddr_in			_address;

	private:
};

class SocketServer : public Socket {
	public:
		SocketServer(char **env, const Config &conf, int connections);

		int		getSocketUsed() const;
		void	setSocketUsed(int fd);
		fd_set	getReadFds() const;
		fd_set	getWriteFds() const;

		void	setUpSocket(const BlockConfig &block, int connections);
		int		acceptSocket(int server_fd);
		void	selectSocket(const BlockConfig &block);
		bool	ready(int fd, fd_set set);
		void	setClientSocket(size_t id);
		void	simultaneousRead(const BlockConfig &block);
		void	runBlock(const BlockConfig &block);
		void	run(const std::vector<BlockConfig> &conf);
		void	closeClean(fd_set *fds);

	private:
		int		_sd;
		int		_max_sd;
		fd_set	_readfds;
		fd_set	_writefds;
};

#endif
