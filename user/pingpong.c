#include "kernel/types.h"
#include "user/user.h"

int main(void) {
	int p[2];
	char buf[4];
	pipe(p);
	write(p[1], "ping", 4);
	int pid = fork();
	if (pid == 0) {
		exit(getpid());
	}
	read(p[0], buf, 4);
	printf("%d: received ping\n", pid);
	write(p[1], "pong", 4);
	// exit(pid);
	read(p[0], buf, 4);
	printf("%d: received pong\n", getpid());
	exit(getpid());
	return 0;
}
