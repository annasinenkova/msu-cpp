#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>

int
main(void)
{
    int fd[2];
    pipe(fd);
    if (fork() == 0) {
        dup2(fd[1], 1);
        close(fd[1]);
        close(fd[0]);
        execlp("cat", "cat", "/etc/passwd", (char*)0);
        exit(1);
    }
    dup2(fd[0], 0);
    close(fd[0]);
    close(fd[1]);
    execlp("grep", "grep", "user", (char*)0);
    exit(1);
}

