# LINUX
	comanda Makefile:
		make main1
		make clean
	
	Biblioteca este: libso_stdio.so
	
	In executabilul main1 se regaseste un test pentru so_popen si so_pclose folosind comanda 'ls -la'.
	
	Buffer-ul folosit in structura _so_file are un buf_size si un _index. buf_size arata indexul ultimului element,
	iar _index reprezinta indexul primului element din buffer.
	Buffer INVALID => _index == buf_size
	
	so_fopen() -> verifica type sa apartina multimii precizate
	->aloca si dezaloca in caz de eroare pe stream
	
	so_fgetc() si so_putc() muta cursorul in fisier, incrementandu-l cu 1 la fiecare operatie.
	
	so_fflush() cursorul se incrementeaza cu restul dimensiunii bufferului daca acesta este valid si ultima operatie a fost de write.
	Daca ultima operatie a fost de read => se invalideaza bufferul.
	
	so_fclose() => free(stream) si so_fflush daca este cazul (ultima operatie write)
	
	Structura _so_file:
	- _popen_called reprezinta daca s-a apelat so_popen() pentru a obtine _so_file *
	- _file este handle-ul fisierului
	- current_cursor reprezinta pozitia curenta a cursorului in fisierului
	- buffer[4096] , buf_size , _index
	- error_code => FINE sau ERROR , flagul este setat la aparitia erorilor (ex: eroare la apelul open(),write(),fork(),malloc()...)
	- open_mode => RD, WR, AP, RDPLUS, WRPLUS, APPLUS
	- last_operation => NOT_USED, READ, WRITE
	- eof => EndOfFile
	- pid => pid-ul procesului copil (daca s-a folosit so_popen())
	

# WINDOWS
	comanda NMakefile:
		nmake main.exe 
		nmake clean
	
	Biblioteca este: libso_stdio.dll
	
	In executabilul main.exe se regaseste un test pentru so_popen si so_pclose folosind comanda 'type main.c'.
	Structura _so_file este asemanatoare cu cea din Linux, difera:
	- PROCESS_INFORMATION pi
