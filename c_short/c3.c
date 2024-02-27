//Длина пароля больше 0.

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include<elf.h>


char *
read_string(void)
{
    int c;
    char *str = malloc(sizeof(*str));
    assert(str != NULL);
    unsigned long len = 0;
    while ((c = getchar()) != '\n') {
        if (c == EOF) {
            free(str);
            return NULL;
        }
        str = realloc(str, (len + 1)*sizeof(*str));
        assert(str != NULL);
        str[len++] = c;
    }
    str = realloc(str, (len + 1)*sizeof(*str));
    assert(str != NULL);
    str[len++] = '\0';
    return str;
}


uint32_t
address_search(void)
{
    char *str, *strfree;
    char *strprev = malloc(sizeof(*str));
    while (!strstr(str = read_string(), "<main>:\0")) {
        free(str);
    }
    free(str);
    while (!strstr(str = read_string(), "<strcmp@plt>\0")) {
        strfree = strprev;
        strprev = str;
        free(strfree);
    }
    uint32_t address = strtol(strrchr(strprev, '$') + 1, NULL, 16);
    free(str);
    free(strprev);
    return address;
}


void
replacing_password(char *name, uint32_t address)
{
    FILE *f = fopen(name, "r+");
    Elf32_Ehdr ehdr;
    fseek(f, 0, SEEK_SET);
    fread(&ehdr, sizeof(ehdr), 1, f);
    fseek(f, ehdr.e_shoff, SEEK_SET);
    Elf32_Shdr section;
    fread(&section, sizeof(section), 1, f);
    while (address < section.sh_addr || address >= (section.sh_addr + section.sh_size)) {
        fread(&section, sizeof(section), 1, f);
    }
    uint32_t offset = address - section.sh_addr + section.sh_offset;
    char c;
    char symbol = 'z';
    fseek(f, offset, SEEK_SET);
    fread(&c, sizeof(char), 1, f);
    while (c != 0) {
        fseek(f, -sizeof(char), SEEK_CUR);
        fwrite(&symbol, sizeof(char), 1, f);
        fread(&c, sizeof(char), 1, f);
        printf("%c", symbol);
    }
    printf("\n");
    fclose(f);
    return;
}


int 
main(int argc, char *argv[])
{
    uint32_t address = address_search();
    replacing_password(argv[1], address);
    return 0;
}
