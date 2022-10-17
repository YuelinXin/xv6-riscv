#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
	if(argc == 1){
		// exit(0);
		write(1, "Usage: sleep <tick>\n", 20);
	} else {
		int tick = atoi(argv[1]);
		if(tick <= 0) {
			// exit(0);
			write(1, "Error: invalid tick time\n", 25);
		} else {
			sleep(tick);
		}
	}
	return 0;
}
