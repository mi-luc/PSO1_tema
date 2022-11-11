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
    DWORD final_offset=SetFilePointer(stream->_file,0,0,FILE_END);
    SetFilePointer(stream->_file,stream->current_cursor,0,FILE_BEGIN);
    //printf("!%d!-*%d*\n",stream->current_cursor,final_offset);
    
    if(final_offset+1==stream->current_cursor)   
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
HANDLE so_fileno(SO_FILE* stream)
{
return stream->_file;
}