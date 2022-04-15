#include "SimpleSocket.hpp"

#include <netinet/in.h>

/*** C includes ***/
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

/*** C++ includes ***/
#include <cstring>
#include <iostream>

#define BUFFER_SIZE 4096
#ifndef TEST
#define PORT 8080
#else
#define PORT TEST
#endif

int	main(int ac, char **av) {
	int		server_fd, new_socket;
	long	valread;
	struct sockaddr_in	address;
	int		addrlen = sizeof(address);

	(void)av;
	if (ac > 1) {
		std::cout << "Too much args" << std::endl;
		return EXIT_FAILURE;
	}
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("In sockets");
		close(server_fd);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	memset(address.sin_zero, '\0', sizeof(address.sin_zero));

	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
		perror("In Bind");
		close(server_fd);
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 10) < 0) {
		perror("In listen");
		exit(EXIT_FAILURE);
	}

//	std::cout << "Content-type:text/html\r\n\r\n"
//		<< "<html>\n"
//		<< "<head>\n"
//		<< "<title>Hello World - First CGI Program</title>\n"
//		<< "</head>\n"
//		<< "<body>\n"
//		<< "<h2>Hello World! This is my first CGI program</h2>\n"
//		<< "</body>\n"
//		<< "</html>\n" << std::endl;

		std::string str = "Content-type:text/html\r\n\r\n<html>\n<head>\n<title>Hello World!/title>\n</head>\n<body>\n<h1>Hello World! This is my first CGI program</h2>\n</body>\n</html>\n";
		char	buffer[BUFFER_SIZE] = {0};

	while (true) {

		std::cout << "\n+++++++ Waiting for new connection ++++++++\n" << std::endl;
		if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
			perror("In accept");
			exit(EXIT_FAILURE);
		}
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
	return EXIT_SUCCESS;

}
