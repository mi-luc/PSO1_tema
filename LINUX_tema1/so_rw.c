#include "so_stdio.h"
#include "so_file.c"
#include <stdio.h>
#include <stdlib.h>
//SCRIE SI CITESTE DE LA OFFSET!!
size_t so_fread(void *ptr, size_t size, size_t nmemb, SO_FILE *stream)
{
    
    if(stream==NULL)
    {
        return SO_EOF;
    }
    
   

    size_t total_size=size*nmemb;
     char* p=( char*)ptr;
    unsigned long elements_read=0;
    for(unsigned long i=0;i<total_size;i++)
    {
     
        p[i]=so_fgetc(stream);
        
        //printf("%d\n",p[i]);
          
        if(p[i]==SO_EOF && stream->eof==EndOfFile)
        {
              
           // printf("E\n");
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
    //POZITIONARE FINAL PENTRU APPEND
   
    
    if(stream->open_mode==AP )
    {
        stream->current_cursor=lseek(stream->_file,0,SEEK_END);
        //printf("APPENDING");
    }
     if(stream->open_mode==APPLUS )
    {
        
        lseek(stream->_file,0,SEEK_END);
        //printf("APPENDING");
    }



    size_t total_size=size*nmemb;
     char* p=( char*)ptr;
    unsigned long elements_written=0;
    for(int i=0;i<total_size;i++)
    {
        
        int char_put=so_fputc(p[i],stream);
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
    stream->current_cursor=lseek(stream->_file,offset,whence);
    if(stream->current_cursor==-1)
    {
    stream->error_code=ERROR;
    return -1;
    }
    return 0;
}

long so_ftell(SO_FILE* stream)
{
    return stream->current_cursor;
}
