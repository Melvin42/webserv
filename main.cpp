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
		std::cout << "Too much args" << std::endl;
		return EXIT_FAILURE;
	}
	try {
		SocketServer server = SocketServer(PORT, 10);
		std::string space = " ";
		std::string cr = "\r\n";

		std::string header;

		header += "HTTP/1.1" + space + "200" + space + "OK" + cr;
		header += "Content-type:text/html" + cr + cr;

		if ((arg_fd = open(av[1], O_RDONLY)) == -1)
			return EXIT_FAILURE;
		char *buf = NULL;
		while ((valread = read(arg_fd, buf, 1)) > 0) {
			if (valread == -1) {
				close(arg_fd);
				return EXIT_FAILURE;
			}
		}

		std::cout << buf;

		std::string body;

		body += "<html>\n";
		body += "<head>\n";
		body += "<title>";
		body += "Hello World!";
		body += "</title>\n";
		body += "</head>\n";
		body += "<body>\n";
		body += "<h1>Hello World!</h1>\n";
		body += "</body>\n";
		body += "</html>\n";

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
