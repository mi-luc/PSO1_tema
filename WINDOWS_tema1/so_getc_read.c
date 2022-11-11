#include "so_stdio.h"
#include "so_file.c"
#include <fileapi.h>
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
    //Seteaza ultimul index ca si dimensiunea-1 sau 0 daca dim e 0;
    if(stream->last_operation==WRITE)
    {
       // printf("LAST WAS WRITE!\n");
        if(stream->open_mode==WRPLUS || stream->open_mode==RDPLUS || stream->open_mode==APPLUS)
        {
           // printf("PREFLUSH\n");
            if(stream->buff_size!=stream->_index)
            {
               // printf("FLUSHING!\n");
                so_fflush(stream);
            }
            //READ DUPA WRTIE SCRIE BUFFERUL in fisier
        }
    }
    if(stream->open_mode==APPLUS)
    {
        //lseek(stream->_file,stream->current_cursor,SEEK_SET);
        SetFilePointer(stream->_file,stream->current_cursor,0,FILE_BEGIN);
    }
    //SETEAZA ULTIMA OPERATIE
    stream->last_operation=READ;
    
    
    
    if(last_index+1!=first_index && last_index!=0)
    {
        
        
        stream->_index=stream->_index+1;
        stream->current_cursor++;
        return (int)stream->buffer[first_index+1];
    }
    else
    {
        
        size_t totalBytesRead=0;
        BOOL status_read=ReadFile(stream->_file,stream->buffer,BUFF_SIZE,&totalBytesRead,NULL);
        
        if(totalBytesRead==0)
        {
           
            stream->current_cursor++;
            stream->eof=EndOfFile;
            stream->error_code=ERROR;
            return SO_EOF;
        }
        if(status_read==FALSE)
        {
            
            stream->error_code=ERROR;
            return SO_EOF;
        }

        stream->buff_size=totalBytesRead-1;
        stream->_index=0;
        stream->current_cursor++;
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
    
    stream->last_operation=WRITE;
    stream->_index=0;
   
   
    if(stream->buff_size<BUFF_SIZE)
    {
        
        stream->buffer[stream->buff_size]=c;
        stream->buff_size=stream->buff_size+1;
         stream->current_cursor++;
        return stream->buffer[stream->buff_size-1];
    }
    else
    {
        stream->_index=0;
        size_t bytesWritten=0;
        while(stream->_index!=stream->buff_size)
        {
        BOOL status_write=WriteFile(stream->_file,stream->buffer+stream->_index,stream->buff_size-stream->_index,&bytesWritten,NULL);
        stream->_index+=bytesWritten;
        
         // printf("curs:%d  %d",stream->current_cursor,stream->_index);
        if(status_write==0)
        {
           
            stream->error_code=ERROR;
            return SO_EOF;
        }
        }
       stream->current_cursor++;
        stream->_index=0;
        stream->buff_size=1;
        stream->buffer[0]=c;
       
        return stream->buffer[0];
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
       
        stream->_index=0;
        size_t bytesWritten=0;
        while(stream->_index!=stream->buff_size)
        {
        
        BOOL status_write=WriteFile(stream->_file,stream->buffer+stream->_index,stream->buff_size-stream->_index,&bytesWritten,NULL);
        stream->_index+=bytesWritten;
       //  stream->current_cursor+=bytesWritten;
        if(status_write==FALSE)
        {
           
            stream->error_code=ERROR;
            return SO_EOF;
        }
        }
        stream->buff_size=0;
        stream->_index=0;
        return 0;
    }
}