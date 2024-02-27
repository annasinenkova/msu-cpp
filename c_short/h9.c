/*
    Если параметр опции -D - varl, то вызывается функция execlp,
    если параметр - varv, то функция execvp.
*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int
main(void)
{
#if(!varl && !varv)
    #error no varl or varv
#endif
#ifdef varl
    execlp("gcc", "gcc", "1.c", "-o", "1.exe", "-lm", (char*)0);
#elif varv
    char *arg[] = {"gcc", "1.c", "-o", "1.exe", "-lm", NULL};
    execvp("gcc", arg);
#endif
    perror("Exec");
    return 0;
}
