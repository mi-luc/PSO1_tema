#include "so_stdio.h"
#include "so_file.c"
#include <string.h>
#include <stdio.h>


#define PIPE_READ 0
#define PIPE_WRITE 1
#define STDOUT_DESCRIPTOR 1
#define STDIN_DESCRIPTOR 0
 SO_FILE *so_popen(const char *command, const char *type)
 {
   
    if(type[1]!='\0')
    {
        return NULL;
    }
    char completeCommand[2000];

    strcpy(completeCommand, "cmd.exe /c ");
	strcat(completeCommand, command);

    HANDLE hReadPipe, hWritePipe;
    //INIT _so_file structure
    SO_FILE* file=(SO_FILE*)malloc(sizeof(SO_FILE));
  
    if(file==NULL)
    {
        return file;
    }
    file->buff_size=0;
    file->_index=0;
    file->current_cursor=0;
    file->_popen_called=POPEN_CALLED;
     
    STARTUPINFO si;
    //INIT
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    GetStartupInfo(&si);
    //INIT
    PROCESS_INFORMATION pi;
    
    ZeroMemory(&pi, sizeof(pi));

    SECURITY_ATTRIBUTES security_atr;
    //INIT
    ZeroMemory(&security_atr, sizeof(security_atr));
    security_atr.nLength = sizeof(SECURITY_ATTRIBUTES);
    security_atr.bInheritHandle = TRUE;

    int pipeStatus=CreatePipe(&hReadPipe, &hWritePipe, &security_atr, 0);

    if(pipeStatus==FALSE)
    {
        free(file);
        return NULL;
    }
     
    switch(type[0])
    {
        case 'r':
        ZeroMemory(&si, sizeof(si));
	    si.cb = sizeof(si);
        si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
	    si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	    si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
        si.dwFlags |= STARTF_USESTDHANDLES;
        si.hStdOutput = hWritePipe;
        file->_file=hReadPipe;
        SetHandleInformation(hReadPipe, HANDLE_FLAG_INHERIT, 0);
            break;
        case 'w':
        ZeroMemory(&si, sizeof(si));
	    si.cb = sizeof(si);
        si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
	    si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	    si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
        si.dwFlags |= STARTF_USESTDHANDLES;
        si.hStdOutput = hReadPipe;
        file->_file=hWritePipe;
        SetHandleInformation(hWritePipe, HANDLE_FLAG_INHERIT, 0);
            break;
        default:
        free(file);
        return NULL;
    }
    BOOL status=CreateProcessA(NULL,completeCommand,NULL,NULL,TRUE,0,NULL,NULL,&si,&pi);
    
  
   switch(type[0])
    {
        case 'r':
            CloseHandle(hWritePipe);
            break;
        case 'w':
           CloseHandle(hReadPipe);
            break;
    }
    if(status==FALSE)
    {
      
        free(file);
        return NULL;
    }

    file->pi=pi;
    //printf("PROCESS ID:%d\n",pi.dwProcessId);
    return file;

 
 }
 int so_pclose(SO_FILE *stream)
 {
   
   
    if(stream==NULL)
    {
        return NULL;
    }
    if(stream->_popen_called=POPEN_CALLED)
    {
       
        if(stream->last_operation==WRITE)
        {
            so_fflush(stream);
        }
        CloseHandle(stream->_file);
        DWORD waitRes = WaitForSingleObject(stream->pi.hProcess, INFINITE);
        free(stream);
        if ( waitRes==WAIT_FAILED )
        {
            return ERROR;
        }
       
        BOOL bRes = GetExitCodeProcess(stream->pi.hProcess, &waitRes);
      
        CloseHandle(stream->pi.hProcess);
        return 0;
    }
    else
    {
        return 0;
    }
 }