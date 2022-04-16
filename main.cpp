/*** C includes ***/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

/*** C++ includes ***/
#include <cstring>
#include <cstdlib>
#include <iostream>

/*** Perso Includes ***/
#include "Sockets.hpp"

int	main(int ac, char **av) {
	int		new_socket, arg_fd;
	long	valread;

	if (ac != 2) {
		std::cout << "Need a file html in second arg" << std::endl;
		return EXIT_FAILURE;
	}
	try {
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

		std::string str = header + body;

		char	buffer[BUFFER_SIZE] = {0};

		while (true) {

			std::cout << "\n+++++++ Waiting for new connection ++++++++\n" << std::endl;

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
	return EXIT_SUCCESS;
}
