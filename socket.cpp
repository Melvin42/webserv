#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 8081

int	main(void) {
	int	server_fd;
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("cannot create socket");
		return 0;
	}
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
	int	new_socket;
	int	addrlen = sizeof(address);
	long	valread;
	const char	*hello = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 115\n\n<html><head><title>Temperature</title></head><body>{\"humidity\":81%,\"airtemperature\":23.5C}</p></body></html>\r\n";
	while (1) {
		if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
			(socklen_t *)&addrlen)) < 0) {
			perror("accept failed");
			exit(EXIT_FAILURE);
		}
		char buffer[4096] = {0};
		valread = read(new_socket, buffer, 4096);
		write(new_socket, hello, strlen(hello));
		close(new_socket);
	}
	return 0;
}
