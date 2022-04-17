#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/poll.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

#define PORT 8080


int	main(void) {
	int	server_fd;
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("cannot create socket");
		return 0;
	}
	int	on = 1;
	int	rc = setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char *) &on,
			sizeof(on));
	if (rc < 0) {
		perror("setsockopt failed");
		close(server_fd);
		exit(-1);
	}
/*	rc = fcntl(server_fd, F_SETFL, fcntl(server_fd, F_GETFL, 0));
	if (rc == -1) {
		perror("fcntl failed");
		close(server_fd);
		exit(-1);
	}*/
	struct sockaddr_in	address;
	memset((char *)&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);
	memset(address.sin_zero, '\0', sizeof(address.sin_zero));
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))) {
		perror("bind failed");
		return 0;
	}
	if (listen(server_fd, 0) < 0) {
		perror("listen failed");
		exit(EXIT_FAILURE);
	}
	struct pollfd	fds[200];
	memset(fds, 0, sizeof(fds));
	fds[0].fd = server_fd;
	fds[0].events = POLLIN;
	int	timeout = 5 * 60 * 1000;
	int	nfds = 1, current_size = 0;
	int	end_server = 0, compress_array = 0;
	int	new_socket = -1;
	int	addrlen = sizeof(address);
	long	valread;
	do {
		printf("Waiting on poll...\n");
		rc = poll(fds, nfds, timeout);
		printf("poll_rc = %d, nfds = %d\n", rc, nfds);
		if (rc < 0) {
			perror("poll failed");
			break;
		}
		if (rc == 0) {
			printf("poll timed out.\n");
			break;
		}
		current_size = nfds;
		for (int i = 0; i < current_size; i++) {
			printf("for i=%d\n", i);
			if (fds[i].revents == 0) {
				printf("revent = 0\n");
				continue;
			}
			if (fds[i].revents != POLLIN) {
				printf("Error: revents = %d\n", fds[i].revents);
				break;
			}
			if (fds[i].fd == server_fd) {
				printf("accept running...\n");
				do {
					new_socket = -1;
					if ((new_socket = accept(server_fd,
							(struct sockaddr *)&address,
							(socklen_t *)&addrlen)) < 0) {
						perror("accept failed");
						end_server = 1;
						break;
					}
					printf("accept_ret = %d, nfds = %d\n", new_socket, nfds);
					fds[nfds].fd = new_socket;
					fds[nfds].events = POLLIN;
					nfds++;
				} while (new_socket != -1);
			} else {
				printf("recv and send...\n");
				int	close_conn = 0;
				do {
					std::string	get_http = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent_Length: ";
					std::string	str_file = "";
					printf("%d\n", new_socket);
					char buffer[4096] = {0};
					rc = recv(fds[i].fd, buffer, sizeof(buffer), 0);
					if (rc < 0) {
						perror("recv failed");
						close_conn = 1;
						break;
					}
					if (rc == 0) {
						printf("Connection closed\n");
						close_conn = 1;
						break;
					}
					int	len = rc;
					printf("%s\n", buffer);
					std::string	request = "";
					for (int i = 0; buffer[i] != '\n' && i < 4096; i++) {
						request += buffer[i];
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
					rc = send(fds[i].fd, str_file.c_str(),
							str_file.length(), 0);
					if (rc < 0) {
						perror("send failed");
						close_conn = 1;
						break;
					}
				} while (1);
				if (close_conn) {
					close(fds[i].fd);
					fds[i].fd = -1;
					compress_array = 1;
				}
			}
		}
		if (compress_array) {
			compress_array = 0;
			for (int j = 0; j < nfds; j++) {
				if (fds[j].fd == -1) {
					for (int k = j; k < nfds - 1; k++) {
						fds[k].fd = fds[k + 1].fd;
					}
					j--;
					nfds--;
				}
			}
		}
	} while (end_server == 0);

	for (int i = 0; i < nfds; i++) {
		if (fds[i].fd != -1)
			close(fds[i].fd);
	}
	return 0;
}
