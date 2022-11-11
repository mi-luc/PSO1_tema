#include "so_file.c"
#include "so_stdio.h"
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
struct _so_file* so_fopen(const char* pathname,const char* mode)
{
    struct _so_file* pointer;
    int descriptor;
    
   
    if(mode[0]=='r' && mode[1]=='+' && mode[2]=='\0')
    {
         descriptor=open(pathname,O_RDWR);
         if(descriptor==-1)
         {
             return NULL;
         }
         pointer=(struct _so_file*)malloc(sizeof(struct _so_file));
        
        pointer->current_cursor=0;
        pointer->buff_size=0;
        pointer->_file=descriptor;
        pointer->_index=0;
        lseek(descriptor,0,SEEK_SET);
        pointer->open_mode=RDPLUS;
        pointer->last_operation=NOT_USED;
        pointer->_popen_called=POPEN_NOT_CALLED;
        pointer->error_code=FINE;
        pointer->pid=0;
         pointer->eof=10;
        return pointer;
    }
     if(mode[0]=='w' && mode[1]=='+' && mode[2]=='\0')
    {
        descriptor=open(pathname,O_RDWR|O_CREAT|O_TRUNC,777);
        if(descriptor==-1)
        {
            return NULL;
        }
        pointer=(struct _so_file*)malloc(sizeof(struct _so_file));
        pointer->current_cursor=0;
        pointer->buff_size=0;
        pointer->_file=descriptor;
        pointer->_index=0;
        lseek(descriptor,0,SEEK_SET);
        pointer->open_mode=WRPLUS;
        pointer->last_operation=NOT_USED;
        pointer->_popen_called=POPEN_NOT_CALLED;
        pointer->error_code=FINE;
        pointer->eof=10;
        pointer->pid=0;
        return pointer;
    }
      if(mode[0]=='a'&& mode[1]=='+' && mode[2]=='\0')
    {
        descriptor=open(pathname,O_RDWR);
        if(descriptor==-1)
        {
            return NULL;
        }
        pointer=(struct _so_file*)malloc(sizeof(struct _so_file));
        pointer->current_cursor=0;
        pointer->buff_size=0;
        pointer->_file=descriptor;
        pointer->_index=0;
        pointer->open_mode=APPLUS;
       pointer->_popen_called=POPEN_NOT_CALLED;
        pointer->last_operation=NOT_USED;
        pointer->error_code=FINE;
        pointer->pid=0;
         pointer->eof=10;
        return pointer;
    }
    if(mode[0]=='r' && mode[1]=='\0')
    {
        descriptor=open(pathname,O_RDONLY);
        if(descriptor==-1)
        {
            return NULL;
        }
        //printf("WORKS\n");
        pointer=(struct _so_file*)malloc(sizeof(struct _so_file));
        pointer->current_cursor=0;
        pointer->buff_size=0;
        pointer->_file=descriptor;
        pointer->_index=0;
        lseek(descriptor,0,SEEK_SET);
        pointer->open_mode=RD;
        pointer->last_operation=NOT_USED;
        pointer->_popen_called=POPEN_NOT_CALLED;
        pointer->error_code=FINE;
        pointer->pid=0;
         pointer->eof=10;
        return pointer;
    }
    
    if(mode[0]=='w' && mode[1]=='\0')
    {
        descriptor=open(pathname,O_WRONLY | O_CREAT|O_TRUNC,777);
        if(descriptor==-1)
        {
            return NULL;
        }
        pointer=(struct _so_file*)malloc(sizeof(struct _so_file));
        pointer->current_cursor=0;
        pointer->buff_size=0;
        pointer->_file=descriptor;
        pointer->_index=0;
        lseek(descriptor,0,SEEK_SET);
        pointer->open_mode=WR;
        pointer->last_operation=NOT_USED;
        pointer->_popen_called=POPEN_NOT_CALLED;
        pointer->error_code=FINE;
        pointer->pid=0;
         pointer->eof=10;
        return pointer;
    }
   
    if(mode[0]=='a' && mode[1]=='\0')
    {
        descriptor=open(pathname,O_WRONLY);
        if(descriptor==-1)
        {
            return NULL;
        }
        
        pointer=(struct _so_file*)malloc(sizeof(struct _so_file));
        pointer->current_cursor=0;
        pointer->buff_size=0;
        pointer->_file=descriptor;
        pointer->_index=0;
        lseek(descriptor,0,SEEK_END);
        pointer->open_mode=AP;
        pointer->last_operation=NOT_USED;
        pointer->_popen_called=POPEN_NOT_CALLED;
        pointer->error_code=FINE;
        pointer->pid=0;
         pointer->eof=10;
        return pointer;
    }
    return NULL;
    

}

int so_fclose(struct _so_file* stream)
{ 
   
if(stream==NULL)
{
    return SO_EOF;
}
if(stream->last_operation==WRITE)
{
   //printf("WRITE BRANCH");
    if(stream->_index!=stream->buff_size )
    {
        
       
        so_fflush(stream);
        
        if(stream->error_code==ERROR)
        {
            memset(stream,0,sizeof(SO_FILE));
            free(stream);
            return SO_EOF;
        }
        //printf("CLOSING %d\n",lseek(stream->_file,0,SEEK_END));
    }
}
//printf("READ BRANCH");
int close_error_code=close(stream->_file);
memset(stream,0,sizeof(SO_FILE));

free(stream);
if(close_error_code==-1)
{
    return SO_EOF;
}


return 0;
}
