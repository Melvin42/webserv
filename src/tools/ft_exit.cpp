#include "../class/Config.hpp"
#include "../class/Sockets.hpp"

class SocketServer;

int	ft_exit(Config &conf, SocketServer &server) {
	(void)conf;
	(void)server;
//	free(conf);
//	free(server);
	std::cout << "exit !!" << std::endl;
	return -1;
//	return 0;
}
