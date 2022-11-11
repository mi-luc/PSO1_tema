#include "so_stdio.h"
#include "so_file.c"
#include <unistd.h>
#include <stdio.h>

int so_fgetc(SO_FILE* stream)
{
    if(stream==NULL)
    {
        return SO_EOF;
    }

    if(stream->open_mode==WR || stream->open_mode==AP)
    {
        stream->error_code=ERROR;
        return SO_EOF;
    }
    stream->eof=10;
    unsigned int first_index=stream->_index;
    unsigned int last_index=0;
    if(stream->buff_size!=0)
    last_index=stream->buff_size-1;
  
    if(stream->last_operation==WRITE)
    {
       // printf("LAST WAS WRITE!\n");
        if(stream->open_mode==WRPLUS || stream->open_mode==RDPLUS || stream->open_mode==APPLUS)
        {
            //printf("PREFLUSH\n");
            if(stream->buff_size!=stream->_index)
            {
                //printf("FLUSHING!\n");
                so_fflush(stream);
            }
            //READ DUPA WRTIE SCRIE BUFFERUL in fisier
        }
    }
    if(stream->open_mode==APPLUS)
    {
        lseek(stream->_file,stream->current_cursor,SEEK_SET);
    }
    //SETEAZA ULTIMA OPERATIE
    stream->last_operation=READ;
    
    
    
    if(last_index+1!=first_index && last_index!=0)
    {
        //printf("INDEXES:%d %d\n",first_index,last_index);
        stream->_index=stream->_index+1;
        //printf("B:%c %d\n",stream->buffer[first_index+1],stream->_index);
        stream->current_cursor++;
      
        return (int)stream->buffer[first_index+1];
    }
    else
    {
        //printf("%d %d EGALITATE\n",last_index,first_index);
        ssize_t bytes_read=read(stream->_file,stream->buffer,BUFF_SIZE);
       
        if(bytes_read<=0)
        {
            if(bytes_read<0)
        {
            stream->error_code=ERROR;
        }
            
           
          
            //printf("EOF AT %d\n",stream->current_cursor);
            stream->eof=EndOfFile;
            return SO_EOF;
        }
        //stream->current_cursor+=bytes_read;
        stream->buff_size=bytes_read-1;
        stream->_index=0;
        //printf("B:%c %d\n",stream->buffer[0],stream->_index);
        
        stream->current_cursor++;
        //printf("\nCURS:%d\n",stream->current_cursor);
        return (int)stream->buffer[0];
    }
    
    
    

}
int so_fputc(int c,SO_FILE* stream)
{
    if(stream==NULL)
    {
        return SO_EOF;
    }
    if(stream->open_mode==RD)
    {
        stream->error_code=ERROR;
        return SO_EOF;
    }
    //ERRORS

    
    //O operatie de scriere dupa citire trebuie facut un fseek
    //Este in grija utilizatorului!
  
    stream->last_operation=WRITE;
    stream->_index=0;

   
    if(stream->buff_size<BUFF_SIZE)
    {
        //printf("B:%c %d\n",c,stream->buff_size);
        
        stream->buffer[stream->buff_size]=c;
        stream->buff_size=stream->buff_size+1;
        stream->current_cursor++;
        return stream->buffer[stream->buff_size-1];
        
    }
    else
    {
        //printf("SE SCRIE IN FISIER!");
        ssize_t bytes_written;
        stream->_index=0;
        while(stream->_index!=stream->buff_size)
        {
         bytes_written=write(stream->_file,stream->buffer+stream->_index,BUFF_SIZE-stream->_index);
         if(bytes_written<0)
        {
          
            stream->error_code=ERROR;
            return;
        }
        if(bytes_written>0)
        {
            
            stream->_index+=bytes_written;
        }
        }
        //printf("%d\n",stream->_index);
        stream->_index=0;
        stream->buff_size=1;
        stream->buffer[0]=c;
        stream->current_cursor++;
        return stream->buffer[0];
        /////
        if(bytes_written<=0)
        {
             if(bytes_written<0)
            {
                stream->error_code=ERROR;
            }
            
            
            return SO_EOF;
        }
       
       // printf("B:%c %d\n",c,stream->buff_size-1);
    }
  
}

int so_fflush(SO_FILE *stream)
{
    if(stream==NULL)
    {
        return SO_EOF;
    }
    if(stream->buff_size)
    {
        char* buffer_start=stream->buffer+stream->_index;
        //unsigned long size_to_flush=stream->buff_size-stream->_index;
        //printf("\n%d size to flush\n",size_to_flush);
       stream->_index=0;
       size_t bytes_written;
        while(stream->_index!=stream->buff_size)
        {
          
         bytes_written=write(stream->_file,stream->buffer+stream->_index,stream->buff_size-stream->_index);
         if(bytes_written==-1)
        {
            stream->error_code=ERROR;
            return SO_EOF;
        }
          if(bytes_written>0)
        {
            stream->current_cursor+=bytes_written;
            stream->_index+=bytes_written;
        }
        else
        {
            stream->error_code=ERROR;
        }
       
        }
       // printf("%d\n",stream->_index);
        stream->_index=0;
          if(bytes_written<=0)
        {
            return SO_EOF;
        }
        stream->buff_size=0;
        stream->_index=0;
        return 0;
    }
}
