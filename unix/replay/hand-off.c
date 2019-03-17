// simplest example of handing off and running process: open the tty, dup
// the fd, fork/exec my-install, wait and print exit code.
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <termios.h>

#include "trace.h"
#include "tty.h"
#include "support.h"
#include "demand.h"

// synchronously wait for <pid> to exit.  Return its exit code.
static int exit_code(int pid) {
	int status;
	if(waitpid(pid, &status, 0) < 0)
		sys_die("waitpid", "waitpid failed?");
    if(!WIFEXITED(status))
        panic("%d: unexpected crash\n", pid);


	return WEXITSTATUS(status);
}

// run:
// 	1. fork
// 	2. in child:
// 		a. remap <fd> to <TRACE_FD_HANDOFF>
//		b. execvp the process in <argv[]>
//	3. in parent: 
//		a. wait for child.
//		b. print its exit code or if it crashed.
//
//  Note: that when you run my-install with tracing, the output
//  should be identical to running it raw.
void run(int fd, char *argv[]) {
	int pid = fork();

	if (pid < 0) sys_die(fork, "bad fork");

	if(pid == 0) {
		if(dup2(fd, TRACE_FD_HANDOFF) < 0) {
			sys_die(dup2, "bad dup2");
		}

		execvp(argv[0], argv);

		sys_die(execvp, "bad execvp");
	}

	fprintf(stderr, "child %d: exited with: %d\n", pid, exit_code(pid));
}

int main(int argc, char *argv[]) {
        // open tty dev, set it to be 8n1  and 115200 baud
	const char *portname = 0;
	int fd = open_tty(&portname);
	fd = set_tty_to_8n1(fd, B115200, 1);

	run(fd, &argv[1]);
	return 0;
}
