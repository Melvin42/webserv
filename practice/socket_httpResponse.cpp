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
#include "HttpRequest.hpp"

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
	struct sockaddr_in	address;
	memset((char *)&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);
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
		std::string	str_file = "";
		if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
						(socklen_t *)&addrlen)) < 0) {
			perror("accept failed");
			exit(EXIT_FAILURE);
		}
		char buffer[4096] = {0};
		if (read(new_socket, buffer, 4096) != 0) {
			HttpRequest	req(buffer, 4096);
			if (req.getPage() == "home.html") {
				HttpResponse	msg;
				str_file = msg.getHttpResponse("home.html");
			} else {
				HttpResponse	msg;
				str_file = msg.getHttpResponse("not_found.html");
			}
			if (send(new_socket, str_file.c_str(), str_file.size(),
						0) == str_file.size())
				close(new_socket);
		}
	}
	return 0;
}
