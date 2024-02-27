/*
    Имя файла передается аргументом командной строке.
    Файл содержит числа типа int.
*/

#include <stdio.h>
#include <stdlib.h>

int
Reverse_file(char *name)
{
    FILE *f = fopen(name, "r+");
    if (f == NULL) {
        return 1;
    }
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    int beg, end;
    for (long i = 0; i < (size/sizeof(int))/2; ++i) {
        fseek(f, i*sizeof(int), SEEK_SET);
        fread(&beg, sizeof(int), 1, f);
        fseek(f, size - (i + 1)*sizeof(int), SEEK_SET);
        fread(&end, sizeof(int), 1, f);
        fseek(f, size - (i + 1)*sizeof(int), SEEK_SET);
        fwrite(&beg, sizeof(int), 1, f);
        fseek(f, i*sizeof(int), SEEK_SET);
        fwrite(&end, sizeof(int), 1, f);
    }
    fseek(f, i*sizeof(int), SEEK_SET);
    int v = 0;
    fputs(v, f);
    fclose(f);
    return 0;
}

void
Print_file(char *name)
{
    FILE *f = fopen(name, "r");
    if (f == NULL) {
        return;
    }
    fseek(f, 0, SEEK_END);
    long size = ftell(f)/sizeof(int);
    fseek(f, 0, SEEK_SET);
    int el;
    for (long i = 0; i < size; ++i) {
        fseek(f, i*sizeof(int), SEEK_SET);
        fread(&el, sizeof(int), 1, f);
        printf("%d ", el);
    }
    printf("\n");
    fclose(f);
    return;
}


int
main(int argc, char *argv[])
{
    if (argc > 1) {
        Print_file(argv[1]);
        if (Reverse_file(argv[1])) {
            printf("The file was not opened\n");
        }
        else {
            Print_file(argv[1]);
        }
    }
    return 0;
}
