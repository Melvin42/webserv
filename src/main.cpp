/*** C includes ***/
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/select.h>
#include <fcntl.h>

/*** C++ includes ***/
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>

/*** Perso Includes ***/
#include "class/Sockets.hpp"
#include "class/Config.hpp"

int	main(int ac, char **av) {
	if (ac > 2) {
		std::cout << "Need a NGINX file.conf as second arg" << std::endl;
		return EXIT_FAILURE;
	} else if (ac == 2) {
		try {
			BlockConfig conf = BlockConfig(av[1]);
			SocketServer server = SocketServer(*conf.getPort().data(), 30);

			server.run();
		} catch (std::exception &e) {
			std::cerr << e.what() << std::endl;
		}
	} else {
		try {
			BlockConfig conf = BlockConfig("index/verysimple.conf");
			SocketServer server = SocketServer(*conf.getPort().data(), 30);

			server.run();
		} catch (std::exception &e) {
			std::cerr << e.what() << std::endl;
		}
	}
	return EXIT_SUCCESS;
}
