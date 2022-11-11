#include "so_file.c"
#include "so_stdio.h"
int so_feof(SO_FILE* stream)
{
    if(stream==NULL)
    {
        return SO_EOF;
    }
    if(stream->_popen_called==POPEN_CALLED)
    {
        //printf("POPEN CALLED");
        if(stream->eof==EndOfFile)   
    {
        return 1;
    }
    return 0;
    }
    else
    {
    
    off_t final_offset=lseek(stream->_file,0,SEEK_END);
    //printf("!%d!-*%d*",stream->current_cursor,final_offset);
    lseek(stream->_file,stream->current_cursor,SEEK_SET);
    if(final_offset==stream->current_cursor)   
    {
        return 1;
    }
    return 0;
    }
}
int so_ferror(SO_FILE* stream)
{
return stream->error_code;
}
int so_fileno(SO_FILE* stream)
{
return stream->_file;
}