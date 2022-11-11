#include "so_stdio.h"
#include "so_file.c"
#include <fileapi.h>
#include <winnt.h>
#include <stdio.h>
SO_FILE* so_fopen(const char* pathname, const char* mode)
{

   struct _so_file* pointer=NULL;
   HANDLE handle=NULL;
    
    pointer=(struct _so_file*)malloc(sizeof(struct _so_file));
    pointer->buff_size=0;
    pointer->_index=0;
    pointer->current_cursor=NOT_EndOfFile;
    pointer->last_operation=NOT_USED;
    pointer->error_code=FINE;
    pointer->_popen_called=POPEN_NOT_CALLED;
    pointer->eof=10;

    if(mode[0]=='r' && mode[1]=='+' && mode[2]=='\0')
    {
         handle=CreateFileA(pathname,GENERIC_READ | GENERIC_WRITE,NULL,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
         if(handle==INVALID_HANDLE_VALUE)
        {
            goto invalid;
        }   

        pointer->_file=handle;
        pointer->open_mode=RDPLUS;
      
        return pointer;
    }
     if(mode[0]=='w' && mode[1]=='+'  && mode[2]=='\0')
    {
        handle=CreateFileA(pathname,GENERIC_READ | GENERIC_WRITE,NULL,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
        if(handle==INVALID_HANDLE_VALUE)
        {
             goto invalid;
        }   

        pointer->_file=handle;
        pointer->open_mode=WRPLUS;
         
        return pointer;
    }
      if(mode[0]=='a'&& mode[1]=='+'  && mode[2]=='\0')
    {
        handle=CreateFileA(pathname,FILE_APPEND_DATA ,FILE_SHARE_READ,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
        if(handle==INVALID_HANDLE_VALUE)
        {
            goto invalid;
        }   

        pointer->_file=handle;
        pointer->open_mode=APPLUS;
        
        return pointer;
    }
    if(mode[0]=='r' && mode[1]=='\0')
    {
        handle=CreateFileA(pathname,GENERIC_READ,FILE_SHARE_WRITE |FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
        if(handle==INVALID_HANDLE_VALUE)
        {
            goto invalid;
        }   

        pointer->_file=handle;
        pointer->open_mode=RD;
        
        return pointer;
    }
    
    if(mode[0]=='w'&& mode[1]=='\0')
    {
        handle=CreateFileA(pathname,GENERIC_WRITE,FILE_SHARE_WRITE |FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
        DWORD err= GetLastError();
       
         if(handle==INVALID_HANDLE_VALUE)
        {
            goto invalid;
        }

        pointer->_file=handle;
        pointer->open_mode=WRITE;
        
        return pointer;
    }
   
    if(mode[0]=='a'&& mode[1]=='\0')
    {
        handle=CreateFileA(pathname,FILE_APPEND_DATA ,FILE_SHARE_READ,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
        if(handle==INVALID_HANDLE_VALUE)
        {
            goto invalid;
           
        }   

        pointer->_file=handle;
        pointer->open_mode=AP;
      
       
        return pointer;
    }
    invalid:
    free(pointer);
    return NULL;
}

int so_fclose(SO_FILE* stream)
{
    
if(stream==NULL)
{
    return SO_EOF;
}

if(stream->last_operation==WRITE)
{
   
    if(stream->_index!=stream->buff_size )
    {
        
       so_fflush(stream);
       CloseHandle(stream->_file);
       free(stream);
       return 0;
    }
}
BOOL close_error_code=CloseHandle(stream->_file);
free(stream);
if(close_error_code==FALSE)
{
    return SO_EOF;
}
return 0;

}