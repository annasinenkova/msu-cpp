#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int count = 0;
int counttrap = 0;

void
sig_int(int s)
{
    if (++count == 2) {
        signal(SIGINT, SIG_DFL);
    }
    else {
        signal(SIGINT, sig_int);
    }
}

void
sig_trap(int s)
{
    if (++counttrap % 2 == 0) {
	int fd[2];
        pipe(fd);
        if (fork() == 0) {
            close(fd[0]);
            close(fd[1]);
            exit(0);
        }
        else if (fork() == 0) {
            close(fd[0]);
            close(fd[1]);
            exit(0);
        }
        else {
            close(fd[0]);
            close(fd[1]);
            wait(NULL);
            wait(NULL);
        }
    }
    signal(SIGTRAP, sig_trap);
}

int
main(void)
{
    signal(SIGINT, sig_int);
    signal(SIGTRAP, sig_trap);
    pid_t pid = getpid();
    while(1) {
        kill(pid, SIGTRAP);
        sleep(1);
    }
    return 0;
}
