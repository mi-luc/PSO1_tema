#include "so_stdio.h"
#include <stdio.h>

int main()
{
    SO_FILE* file;

    printf("POPEN-PCLOSE TEST\n\n"); 
    char buffer[200];
    file=so_popen("ls -la","r");
    so_fread(buffer,1,200,file);
    buffer[200]='\0';
    printf("%s\n",buffer);
    so_pclose(file);
    
   
   
    return 0;
}
