/* 
    Параметры командной строки передаются в порядке: 
        имя функции, a, b, h.
    Параметры a, b, h типа double, b >= a, h > 0.
    Если a и b совпадают, то печатается значение 
        функции в этой точке.
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef double (*ft)(double);

struct fnp {
    const char *name;
    ft ptr;    
};

void
print_table(ft fun, double a, double b, double h)
{
    while (a <= b) {
        printf("%lf ", fun(a));
        a += h;
    }
    printf("\n");
    return;
}

int
main(int argc, char *argv[])
{
    struct fnp m[7] = {{"sin", sin}, {"cos", cos}, {"tan", tan}, {"log", log}, 
                        {"fabs", fabs}, {"sqrt", sqrt}, {"exp", exp}};
    double a, b, h;
    if (argc < 5) {
        printf("Неверное количество параметров!\n");
        exit(1);
    }
    if (sscanf(argv[2], "%lf", &a) < 1) {
        printf("Параметр a неверный!\n");
        exit(1);
    }
    if (sscanf(argv[3], "%lf", &b) < 1 || b < a) {
        printf("Параметр b неверный!\n");
        exit(1);
    }
    if (!sscanf(argv[4], "%lf", &h) || h <= 0.0) {
        printf("Параметр h неверный!\n");
        exit(1);
    }
    int f = 0;
    for (int i = 0; i < 7; ++i) {
        if (!strcmp(argv[1], m[i].name)) {
            print_table(m[i].ptr, a, b, h);
            f = 1;
        }
    }
    if (!f) {
        printf("Неверное имя функции!\n");
        exit(1);
    }
    return 0;
}

