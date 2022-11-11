#include "so_stdio.h"
#include "so_file.c"
#include <stdio.h>
//SCRIE SI CITESTE DE LA OFFSET!!
size_t so_fread(void *ptr, size_t size, size_t nmemb, SO_FILE *stream)
{
    if(stream==NULL)
    {
        return SO_EOF;
    }
    if(size==0)
    {
        return SO_EOF;
    }
      if(stream->open_mode==APPLUS )
    {
        
       stream->current_cursor= SetFilePointer(stream->_file,0,stream->current_cursor,FILE_BEGIN);
         if(stream->current_cursor==INVALID_SET_FILE_POINTER)
        {
            return SO_EOF;
        }
       
    }
   

    long total_size=size*nmemb;
    char* p=(char*)ptr;
    unsigned long elements_read=0;
    for(int i=0;i<total_size;i++)
    {
       
        p[i]=so_fgetc(stream);
       
       if(p[i]==SO_EOF && stream->eof==EndOfFile)
        {
              
            break;
        }
        elements_read++;
    }
    return elements_read/size;
}


size_t so_fwrite(const void *ptr, size_t size, size_t nmemb, SO_FILE *stream)
{
    if(stream==NULL)
    {
        return SO_EOF;
    }
     if(size==0)
    {
        return SO_EOF;
    }
   
    if(stream->open_mode==AP )
    {
        
       
        stream->current_cursor=SetFilePointer(stream->_file,0,0,FILE_END);
        if(stream->current_cursor==INVALID_SET_FILE_POINTER)
        {
            return SO_EOF;
        }
        //printf("%d cursor!\n",SetFilePointer(stream->_file,0,NULL,FILE_CURRENT));
        
    }
     if(stream->open_mode==APPLUS )
    {
        
        SetFilePointer(stream->_file,0,0,FILE_END);
         if(stream->current_cursor==INVALID_SET_FILE_POINTER)
        {
            return SO_EOF;
        }
       
    }
   
    long total_size=size*nmemb;
   char* p=( char*)ptr;
    unsigned long elements_written=0;
    for(int i=0;i<total_size;i++)
    {
        
        int char_put=so_fputc(p[i],stream);
        //printf("index%d: %c",i,p[i]);
        
         elements_written++;
    }
    return elements_written/size;
}

int so_fseek(SO_FILE* stream,long offset,int whence)
{
    if(stream->last_operation==READ)
    {
        stream->_index=0;
        stream->buff_size=0;
    }
    if(stream->last_operation==WRITE)
    {
       so_fflush(stream);
    }
    
    stream->current_cursor=SetFilePointer(stream->_file,offset,0,whence);
    if(stream->current_cursor==INVALID_SET_FILE_POINTER)
    {
    stream->error_code=ERROR;
    return SO_EOF;
    }
    return 0;
}

long so_ftell(SO_FILE* stream)
{
    return stream->current_cursor;
}