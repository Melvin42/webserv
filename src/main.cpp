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
#include "server/Sockets.hpp"
#include "config/BlockConfig.hpp"
#include "config/Config.hpp"

int	main(int ac, char **av, char **env) {
	if (ac > 2) {
		std::cout << "Need a NGINX file.conf as second arg" << std::endl;
		return EXIT_FAILURE;
	} else if (ac == 2) {
			Config conf(av[1]);
			SocketServer server(env, conf, 30);

			server.run();
	} else {
			Config conf("index/simple.conf");
			SocketServer server(env, conf, 30);

			server.run();
	}
	return EXIT_SUCCESS;
}
