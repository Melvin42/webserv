/*** C includes ***/
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/select.h>
#include <fcntl.h>

/*** C++ includes ***/
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>

/*** Perso Includes ***/
#include "Sockets.hpp"
#include "HttpResponse.hpp"
#include "HttpRequest.hpp"

int	main(int ac, char **av) {
	std::vector<int>::iterator			it;
	std::vector<int>::const_iterator	ite;
	char	buffer[BUFFER_SIZE + 1] = {0};
	int		new_socket = 0;
	long	valread = 0;

	(void)av;
	(void)ac;
	//	if (ac != 2) {
	//		std::cout << "Need a file html in second arg" << std::endl;
	//		return EXIT_FAILURE;
	//	}
	try {
		SocketServer server = SocketServer(PORT, 30);

		while (true) {
			std::cout << "\n+++++++ Waiting for new connection ++++++++\n" << std::endl;
			server.selectSocket();
			if (server.ready(server.getMasterFd(), server.getReadFds())) {
				new_socket = server.acceptSocket();
				std::cerr << "New connection, socket fd is " << new_socket << std::endl;
				for (it = server.getClientSocket().begin(); it != ite; it++) {
					if (*it == 0) {
						*it = new_socket;
						break ;
					}
				}
			}
			ite = server.getClientSocket().end();
			std::string	str_file = "";
			for (it = server.getClientSocket().begin(); it != ite; it++) {
				server.setSocketUsed(*it);
				if (server.ready(server.getSocketUsed(), server.getReadFds())) {
					if ((valread = read(server.getSocketUsed(), buffer, BUFFER_SIZE)) == 0) {
						// maybe with POST: server.closeClean();
					} else {
						HttpRequest	req(buffer, BUFFER_SIZE);
						HttpResponse	msg;
						str_file = msg.getHttpResponse(req.getPage());
						if (send(server.getSocketUsed(), str_file.c_str(),
									str_file.size(), 0) == static_cast<long>(str_file.size())) {
							server.closeClean();
							*it = 0;
						}
					}
				}
			}
		}
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	return EXIT_SUCCESS;
}
