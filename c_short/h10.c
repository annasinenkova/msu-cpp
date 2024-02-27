/*
      Файлы f1 и f2 создаются с такими же правами доступа,
    как у исходного файла f.
      Имена файлов задаются в командной строке: сначала имя
    исходного файла f, затем файла f1, затем файла f2.
 */

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>


int
main(int argc, char* argv[])
{
    char buf[2048];
    int f, f1, f2;
    struct stat s;
    if ((f = open(argv[1], O_RDONLY)) == -1) {
        printf("Ошибка открытия файла\n");
        return -1;
    }
    stat(argv[1], &s);
    if ((f1 = creat(argv[2], s.st_mode)) == -1) {
        printf("Ошибка создания файла\n");
        return -1;
    }
    if ((f2 = creat(argv[3], s.st_mode)) == -1) {
        printf("Ошибка создания файла\n");
        return -1;
    }
    int n;
    int i = 0;
    int numstr = 0;
    while (numstr < 100 && (n = read(f, buf, 2048)) > 0) {
        for (i = 0; i < n && numstr < 100; ++i) {
            if (buf[i] == '\n') {
                ++numstr;
            }
        }
        write(f1, buf, i);
    }
    write(f2, buf + i, n - i);
    while ((n = read(f, buf, 2048)) > 0) {
        write(f2, buf, n);
    }
    return 0;
}
