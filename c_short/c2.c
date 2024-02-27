#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

int
main(int argc, char *argv[])
{
    int fd[2];
    pipe(fd);
    if (fork() == 0) {
        dup2(fd[1], 1);
        close(fd[0]);
        close(fd[1]);
        if (fork() == 0) {
            execlp(argv[1], argv[1], NULL);
            perror("exec 1");
            exit(1);
        }
        wait(NULL);
        execlp(argv[2], argv[2], NULL);
        perror("exec 2");
        exit(1);
    }
    close(fd[1]);
    if (fork() == 0) {
        int f;
        if ((f = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1) {
            perror("open");
            exit(2);
        }
        dup2(f, 1);
        close(f);
        dup2(fd[0], 0);
        execlp(argv[3], argv[3], NULL);
        perror("exec 3");
        exit(1);
    }
    close(fd[0]);
    while (wait(NULL) != -1);
    return 0;
}

