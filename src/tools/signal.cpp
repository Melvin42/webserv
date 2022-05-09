#include <csignal>
#include "../class/define.h"

void	signal_handler(int signum) {
	std::cerr << "sig hand" << std::endl;
	if (signum == SIGINT) {
		std::cerr << "sigint" << std::endl;
		g_run = false;
		exit(2);
	}
}
