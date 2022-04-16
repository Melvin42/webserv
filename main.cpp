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

/*** Perso Includes ***/
#include "Sockets.hpp"

int	main(int ac, char **av) {
//	int		ready;
	int		new_socket, arg_fd = 0;
	long	valread;
//	fd_set	readfds, writefds;
//	struct timeval	*pto;

	if (ac != 2) {
		std::cout << "Need a file html in second arg" << std::endl;
		return EXIT_FAILURE;
	}
	try {
//		pto = NULL;
//		FD_ZERO(&readfds);
//		FD_ZERO(&writefds);
//		FD_SET(arg_fd, &readfds);

//		int	flags = fcntl(arg_fd, F_GETFL, 0);
//		fcntl(arg_fd, F_SETFL, flags | O_NONBLOCK);
		
		SocketServer server = SocketServer(PORT, 10);

		std::string header;

		header += "HTTP/1.1 200 OK\r\n";
		header += "Content-type:text/html\r\n\r\n";

		if ((arg_fd = open(av[1], O_RDONLY)) == -1)
			return EXIT_FAILURE;

		char body[BUFFER_SIZE];

		while ((valread = read(arg_fd, body, BUFFER_SIZE)) != 0) {
			if (valread == -1) {
				close(arg_fd);
				return EXIT_FAILURE;
			}
		}
		if (valread == 0)
			close(arg_fd);

		std::string str = header + body;

		char	buffer[BUFFER_SIZE] = {0};

		while (true) {

			std::cout << "\n+++++++ Waiting for new connection ++++++++\n" << std::endl;

//			ready = select(1, &readfds, &writefds, NULL, pto);
//			if (ready == -1) {
//				std::cerr << "Bad select" << std::endl;
//				exit(EXIT_FAILURE);
//			}

			new_socket = server.Accept();

			write(new_socket, str.c_str(), str.size());
			valread = read(new_socket, buffer, BUFFER_SIZE);
			if (valread < 0) {
				std::cerr << "In read" << std::endl;
				exit(EXIT_FAILURE);
			}
			close(new_socket);
			new_socket = 0;
		}
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
//	close(arg_fd);
	return EXIT_SUCCESS;
}
