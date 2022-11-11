#include <unistd.h>
#define __linux__

#define cursor off_t
#define BUFF_SIZE 4096

//Coduri de eroare
#define ERROR -1
#define FINE 0

//Moduri deschidere fisier
#define RD 0
#define RDPLUS 1
#define WR 2
#define WRPLUS 3
#define AP 4
#define APPLUS 5

//Ultima operatie
#define NOT_USED -1
#define READ 1
#define WRITE 2

//so_popen() called
#define POPEN_CALLED 0
#define POPEN_NOT_CALLED 1

//Cursorul este la finalul fisierului
#define EndOfFile -1
struct _so_file
{
    int _popen_called;
    int _file;
    cursor current_cursor;
    char buffer[BUFF_SIZE];
    unsigned int buff_size;
    unsigned int _index;
    int error_code;
    int open_mode;
    int last_operation;
    int eof;
    int pid;
};
