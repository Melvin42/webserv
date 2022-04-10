#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <cstring>

#define MAX_MSG 100
#define MSG_ARRAY_SIZE (MAX_MSG + 3)
#define PORT 8080

int	main() {

	int	listenSocket;
	listenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket < 0) {
		std::cerr << "cannot create socket\n";
		exit(1);
	}

	struct sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddress.sin_port = htons(PORT);
	if (bind(listenSocket, (struct sockaddr *) &serverAddress,
				sizeof(serverAddress)) < 0) {
		std::cerr << "bind failed\n";
		exit(1);
	}


	listen(listenSocket, 0);
	std::cout << "Attente de requete sur le port: " << PORT << "\n";

	socklen_t	clientAddressLength;
	struct sockaddr_in	clientAddress;
	const char *msg = "HTTP/1.0 200 OK\r\nServer: CPi\r\nContent-type: text/html\r\n\r\n<html><head><title>Temperature</title></head><body>{\"humidity\":81%, \"airtemperature\":23.5C}</p></body></html>\r\n";
	char	buffer[2048];
	while (1) {
		int	client_fd = accept(listenSocket,
				(struct sockaddr *) &clientAddress, &clientAddressLength);
		if (client_fd > 0) {
			int	n = read(client_fd, buffer, 2048);
			n = write(client_fd, msg, strlen(msg));
		}
		/*
		clientAddressLength = sizeof(clientAddress);
		memset(msg, 0x0, MSG_ARRAY_SIZE);
		if (recvfrom(listenSocket, msg, MSG_ARRAY_SIZE, 0,
					(struct sockaddr *) &clientAddress,
					&clientAddressLength) < 0) {
			std::cerr << "recv failed\n";
			exit(1);
		}
		if (sendto(listenSocket, msg, strlen(msg) + 1, 0,
					(struct sockaddr *) &clientAddress,
					sizeof(clientAddress)) < 0)
			std::cerr << "send failed\n";
			*/
	}
	close(listenSocket);
	return 0;
}
