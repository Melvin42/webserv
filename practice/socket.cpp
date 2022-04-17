#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include "HttpResponse.hpp"

#define PORT 8080

int	main(void) {
	int	server_fd;
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("cannot create socket");
		return 0;
	}
	int	on = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char *) &on,
				sizeof(on)) < 0) {
		perror("setsockopt failed");
		close(server_fd);
		exit(-1);
	}
	/*	if (fcntl(server_fd, F_SETFL, O_NONBLOCK) == -1) {
		perror("fcntl failed");
		close(server_fd);
		exit(-1);
		}*/
	struct sockaddr_in	address;
	memset((char *)&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);
	//	memset(address.sin_zero, '\0', sizeof(address.sin_zero));
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == -1) {
		perror("bind failed");
		return 0;
	}
	if (listen(server_fd, 0) < 0) {
		perror("listen failed");
		exit(EXIT_FAILURE);
	}
	int	new_socket;
	int	addrlen = sizeof(address);
	long	valread;
	while (1) {
		std::string	get_http = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: ";
		std::string	str_file = "";
		if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
						(socklen_t *)&addrlen)) < 0) {
			perror("accept failed");
			exit(EXIT_FAILURE);
		}
		printf("%d\n", new_socket);
		char buffer[4096] = {0};
		std::string	request = "";
		if (read(new_socket, buffer, 4096) != 0) {
			printf("%s\n", buffer);
			for (int i = 0; buffer[i] != '\n' && i < 4096; i++) {
				request += buffer[i];
			}
		}
		if (request == "GET /home.html HTTP/1.1\r") {
			std::ifstream	fichier_in("home.html");
			str_file = std::string(
					std::istreambuf_iterator<char>(fichier_in),
					std::istreambuf_iterator<char>());
		} else {
			str_file = "Hello World!";
		}

		std::stringstream	len_content;
		len_content << str_file.size();
		std::string	read_len = len_content.str();;

		get_http += read_len;
		get_http += "\n\n";
		get_http += str_file;
		std::cout << get_http << "\n";
		std::cout << new_socket << "\n";
		if (send(new_socket, get_http.c_str(), get_http.size(),
					0) == get_http.size())
			close(new_socket);
	}
	return 0;
}
