.PHONY: clean
main1: libso_stdio.so
	gcc main.c -o main1 -lso_stdio -L .

libso_stdio.so: so_open.o so_file.o so_getc_read.o so_rw.o so_other_functions.o so_proc.o
	gcc -shared so_other_functions.o so_open.o so_file.o so_getc_read.o so_rw.o so_proc.o -o libso_stdio.so

so_open.o: so_open.c
	gcc -fPIC -c so_open.c
	
so_file.o: so_file.c
	gcc -fPIC -c so_file.c

so_getc_read.o: so_getc_read.c
	gcc -fPIC -c so_getc_read.c
so_rw.o: so_rw.c
	gcc -fPIC -c so_rw.c
so_other_functions.o: so_other_functions.c
	gcc -fPIC -c so_other_functions.c
so_proc.o: so_proc.c
	gcc -fPIC -c so_proc.c
clean:
	rm main1 libso_stdio.so *.o 2>/dev/null
