/*** C includes ***/
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <netinet/in.h>

/*** C++ includes ***/
#include <cstring>
#include <iostream>

/*** Perso Includes ***/
#include "Sockets.hpp"

int	main(int ac, char **av) {
	int		new_socket;
	long	valread;

	(void)av;
	if (ac > 1) {
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
//		if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
			new_socket = server.Accept();

			write(new_socket, str.c_str(), str.size());
			valread = read(new_socket, buffer, BUFFER_SIZE);
			if (valread < 0) {
				perror("In read");
				exit(EXIT_FAILURE);
			}
			close(new_socket);
			new_socket = 0;
			std::cerr << new_socket;
		}
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	return EXIT_SUCCESS;
}
