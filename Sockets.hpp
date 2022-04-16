#ifndef SOCKETS_HPP
#define SOCKETS_HPP

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <unistd.h>

#include "define.h"

class Socket {
	public:

		Socket(const Socket &socket);
		virtual ~Socket();

		std::string	ReceiveLine();
		void	Close();
		void	SendLine(std::string);

		Socket &operator=(const Socket &socket);

	protected:
		friend class SocketServer;
		friend class SocketClient;

		Socket(int server_fd);
		Socket();

		int					_server_fd;
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

		int	Accept();
};

/*
class SocketSelect {
	// http://msdn.microsoft.com/library/default.asp?url=/library/en-us/winsock/wsapiref_2tiq.asp
	SocketSelect(Socket const * const s1, Socket const * const s2 = NULL);

	bool Readable(Socket const * const s1);

	private:
		fd_set	_fds;
};
*/

#endif
