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

int	main(int ac, char **av) {
	std::vector<int>::iterator			it;
	std::vector<int>::const_iterator	ite;
	char buffer[2049] = {0};
	int		new_socket = 0;
	long	valread = 0;

	if (ac != 2) {
		std::cout << "Need a file html in second arg" << std::endl;
		return EXIT_FAILURE;
	}
	try {
		SocketServer server = SocketServer(PORT, 30);

		while (true) {
			std::cout << "\n+++++++ Waiting for new connection ++++++++\n" << std::endl;
			server.Select();
			if (server.Ready(server.GetMasterFd(), server.GetReadFds())) {
				new_socket = server.Accept();
				std::cerr << "New connection, socket fd is " << new_socket << std::endl;
			}
			for (it = server.GetClientSocket().begin(); it != ite; it++) {
				if (*it == 0) {
					*it = new_socket;
					break ;
				}
			}
			ite = server.GetClientSocket().end();
			for (it = server.GetClientSocket().begin(); it != ite; it++) {
				server.SetSocketUsed(*it);
				if (server.Ready(server.GetSocketUsed(), server.GetReadFds())) {
					if ((valread = read(server.GetSocketUsed(), buffer, 2048)) == 0) {
						std::cout << "Fichier lu" << std::endl;
					}
				} else {
					std::string	message;

					message += "HTTP/1.1 200 OK\n";
					message += "Content-Type: text/html\n";
//					message += "Content_Length: ";
					std::ifstream	fichier_in(av[1]);
					std::string	body = std::string(
							std::istreambuf_iterator<char>(fichier_in),
							std::istreambuf_iterator<char>());

					std::stringstream	len_body;
					len_body << body.size();
					std::string	str_body_len = len_body.str();
					message += str_body_len;
					message += "\n\n";
					message += body + '\0';
					buffer[valread] = '\0';
					send(server.GetSocketUsed(), message.c_str(), message.length(), 0);
					server.CloseClean();
					*it = 0;
				}
			}
		}
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	return EXIT_SUCCESS;
}
