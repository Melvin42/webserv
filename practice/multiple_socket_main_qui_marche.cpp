#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#define PORT 8080

int main(int argc , char *argv[])
{
	int opt = true;
	int master_socket, addrlen, new_socket, client_socket[30],
	max_clients = 30, activity, i, valread, sd;
	int max_sd;
	struct sockaddr_in address;
	char buffer[2048];
	fd_set readfds;

	for (i = 0; i < max_clients; i++) {
		client_socket[i] = 0;
	}

	if ((master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
			sizeof(opt)) < 0 ) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );

	if (bind(master_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	printf("Listener on port %d \n", PORT);

	if (listen(master_socket, 0) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}

	addrlen = sizeof(address);
	puts("Waiting for connections ...");

	while(true) {
		printf("while runniing\n");
		FD_ZERO(&readfds);

		FD_SET(master_socket, &readfds);
		max_sd = master_socket;

		for (i = 0 ; i < max_clients ; i++) {
			sd = client_socket[i];
			if(sd > 0)
				FD_SET(sd , &readfds);
			if(sd > max_sd)
				max_sd = sd;
		}

		activity = select( max_sd + 1 , &readfds, NULL, NULL , NULL);

		if ((activity < 0) && (errno!=EINTR))
			printf("select failed");
		if (FD_ISSET(master_socket, &readfds)) {
			if ((new_socket = accept(master_socket,
					(struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0){
				perror("accept");
				exit(EXIT_FAILURE);
			}
			printf("New connection, socket fd is %d , ip is : %s , port : %d\n",
					new_socket , inet_ntoa(address.sin_addr),
					ntohs(address.sin_port));

//			if(send(new_socket, "HTTP/1.1 200 OK\n", 16, 0) != 12) {
//				perror("send");
//			}

			for (i = 0; i < max_clients; i++) {
				if(client_socket[i] == 0) {
					client_socket[i] = new_socket;
					printf("Adding to list of sockets as %d\n" , i);
					break;
				}
			}
		}

		for (i = 0; i < max_clients; i++) {
			sd = client_socket[i];

//			if (sd!=0){
				if (FD_ISSET(sd , &readfds)) {
					if ((valread = read(sd, buffer, 2047)) == 0) {
						getpeername(sd, (struct sockaddr*)&address,
								(socklen_t*)&addrlen);
						printf("Host disconnected , ip %s , port %d \n" ,
								inet_ntoa(address.sin_addr), ntohs(address.sin_port));
//						close(sd);
//						FD_CLR(sd, &readfds);
//						client_socket[i] = 0;
					} else {
						std::string	message;
						message += "HTTP/1.1 200 OK\n";
						message += "Content-Type: text/html\n";
//						message += "Content_Length: ";
//
						std::ifstream	fichier_in("home.html");
						std::string	body = std::string(
								std::istreambuf_iterator<char>(fichier_in),
								std::istreambuf_iterator<char>());

						std::stringstream	len_body;
						len_body << body.size();
						std::string	str_body_len = len_body.str();
//
						message += str_body_len;
						message += "\n\n";
						message += body + '\0';
//
//
//						std::cout << message << "\n";
						buffer[valread] = '\0';
						printf("%s\n", buffer);
//						send(sd, buffer, strlen(buffer), 0 );
//						if (send(sd, message.c_str(), message.length(), 0) == message.length()) {
						send(sd, message.c_str(), message.length(), 0);
							close(sd);
							FD_CLR(sd, &readfds);
							client_socket[i] = 0;
//						}
//											sd = 0;
					}
				}
//			}
		}
	}
	return 0;  
}
