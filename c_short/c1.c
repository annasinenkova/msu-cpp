#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int fd1[2], fd2[2];
pid_t pid1, pid2;
int num, count;
char c;

void
sig_int(int s)
{
    kill(0, SIGKILL);
}

void
sig_usr1(int s)
{
    signal(SIGUSR1, sig_usr1);
    if (c == 'z') {
        c = 'a' - 1;
    }
    ++c;
    ++num;
    printf("pid = %d, num = %d\n", getpid(), num);
    printf("pid = %d, c = %c\n", getpid(), c);
    write(fd1[1], &num, sizeof(num));
    write(fd2[1], &c, sizeof(c));
    kill(pid1, SIGUSR2);
    read(fd1[0], &num, sizeof(num));
    kill(pid2, SIGUSR2);
    read(fd2[0], &c, sizeof(c));
    kill(getpid(), SIGUSR1);
}

void
sig_usr2(int s)
{
    signal(SIGUSR2, sig_usr2);
    if (pid1 == 0) {
        read(fd1[0], &num, sizeof(num));		
		++num;
		printf("pid = %d, num = %d\n", getpid(), num);
		write(fd1[1], &num, sizeof(num));
    }
    else if (pid2 == 0) {
        read(fd2[0], &c, sizeof(c));
        if (c == 'z') {
            c = 'a' - 1;
        }
        ++c;
        printf("pid = %d, c = %c\n", getpid(), c);
        write(fd2[1], &c, sizeof(c));
    }
}

int
main(void)
{
    pipe(fd1);
    signal(SIGINT, sig_int);
    signal(SIGUSR1, sig_usr1);
    signal(SIGUSR2, sig_usr2);
    if ((pid1 = fork()) == 0) {
        for(;;);
        return 0;
    }
    pipe(fd2);
    if ((pid2 = fork()) == 0) {
        close(fd1[0]);
        close(fd1[1]);
        for(;;);
        return 0;
    }
    count = 0;
    num = 0;
    c = 'a' - 1;
    kill(getpid(), SIGUSR1);
    while (1);
    return 0;
}

