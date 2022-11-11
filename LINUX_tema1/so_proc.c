#include "so_stdio.h"
#include "so_file.c"
#include <string.h>
#include <stdio.h>
 #include <sys/wait.h>

#define PIPE_READ 0
#define PIPE_WRITE 1
#define STDOUT_DESCRIPTOR 1
#define STDIN_DESCRIPTOR 0
 SO_FILE *so_popen(const char *command, const char *type)
 {
    
    int pipe_fd[2];

   
    
    int status_pipe=pipe(pipe_fd);
    if(status_pipe<0)
        return NULL;
    int pid=fork();
    
    
    if(pid<0)
    {
        return NULL;
    }
    if(pid==0)
    {
        //NE AFLAM IN COPIL
       
    
       if(type[0]=='r' && type[1]=='\0')
    {
         close(pipe_fd[PIPE_READ]);
        int ret=dup2(pipe_fd[PIPE_WRITE],STDOUT_DESCRIPTOR);
         if(ret==-1)
    {
         close(pipe_fd[PIPE_WRITE]);
         exit(1);
        
    }
       
    }
    else if(type[0]=='w' && type[1]=='\0')
    {
         close(pipe_fd[PIPE_WRITE]);
        int ret=dup2(pipe_fd[PIPE_READ],STDIN_DESCRIPTOR);
       if(ret==-1)
    {
        close(pipe_fd[PIPE_READ]);
        exit(1);
    }
    }
    else
    {
        close(pipe_fd[PIPE_WRITE]);
        close(pipe_fd[PIPE_READ]);
        return NULL;
    }


   
        char* args[]={"sh","-c",NULL,NULL};
        args[2]=(char*)command;
        execvp(args[0],args);
        //return NULL;
       
      
		
    }
    else
    {
        struct _so_file* file=(struct _so_file*)malloc(sizeof(struct _so_file));
        file->current_cursor=0;
        file->buff_size=0;
        file->_index=0;
        file->last_operation=NOT_USED;
        file->error_code=FINE;
        file->eof=10;
        file->_popen_called=POPEN_CALLED;
        file->pid=pid;
        //NE AFLAM IN PARINTE
    if(type[0]=='r' && type[1]=='\0') 
    {
        close(pipe_fd[PIPE_WRITE]);
       
        file->_file=pipe_fd[PIPE_READ];
        
        file->open_mode=RD;
        
       
        return file;
    }  

    if(type[0]=='w' && type[1]=='\0')
    {
        close(pipe_fd[PIPE_READ]);
        
        file->_file=pipe_fd[PIPE_WRITE];
        
        file->open_mode=WR;
       

        return file;
    }   
    free(file);

    }
    return NULL;
 
 }
 int so_pclose(SO_FILE *stream)
 {
   
  if(stream==NULL)
  {
    return SO_EOF;
  }
  if(stream->pid==0)
  {
    return SO_EOF;
  }

  int child_pid=stream->pid;
  

    if(stream->_popen_called==POPEN_CALLED)
    {
       
        int status;
        //printf("stream->pid %d\n",stream->pid);
        int wait_return_val=waitpid(child_pid,&status,0);
       
        //printf("wait status:%d\n",status);
        if(stream->last_operation==WRITE)
        {
            so_fflush(stream);
           
        }
        close(stream->_file);
        free(stream);
        if(wait_return_val<0)
        {
           
            return SO_EOF;
        }
        return status;
    }
    else
    {
        return SO_EOF;
    }
 }