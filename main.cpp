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
			server.Select();
			if (server.Ready(server.GetMasterFd(), server.GetReadFds())) {
				new_socket = server.Accept();
				std::cerr << "New connection, socket fd is " << new_socket << std::endl;
				for (it = server.GetClientSocket().begin(); it != ite; it++) {
					if (*it == 0) {
						*it = new_socket;
						break ;
					}
				}
			}
			ite = server.GetClientSocket().end();
			std::string	str_file = "";
			for (it = server.GetClientSocket().begin(); it != ite; it++) {
				server.SetSocketUsed(*it);
				if (server.Ready(server.GetSocketUsed(), server.GetReadFds())) {
					if ((valread = read(server.GetSocketUsed(), buffer, BUFFER_SIZE)) == 0) {
						// maybe with POST: server.closeClean();
					} else {
						HttpRequest	req(buffer, BUFFER_SIZE);
						if (req.getPage() == "webpages/home.html") {
							HttpResponse	msg;
							str_file = msg.getHttpResponse("webpages/home.html");
						} else if (req.getPage() == "webpages/lien.html") {
							HttpResponse	msg;
							str_file = msg.getHttpResponse("webpages/lien.html");
						} else {
							HttpResponse	msg;
							str_file = msg.getHttpResponse("webpages/not_found.html");
						}

						if (send(server.GetSocketUsed(), str_file.c_str(),
									str_file.size(), 0) == static_cast<long>(str_file.size())) {
							server.CloseClean();
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
