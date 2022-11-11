#include "so_stdio.h"
#include <stdio.h>
#include <string.h>
int main()
{
    //TEST so_popen() si so_pclose()
    SO_FILE* file;
    char reading_buff[200];
    file=so_popen("type main.c","r");
    so_fread(reading_buff,1,200,file);
    reading_buff[199]='\0';
    printf("Result:\n%s",reading_buff);
    so_pclose(file);
    return 0;
}