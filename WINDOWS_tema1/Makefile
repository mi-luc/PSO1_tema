.PHONY: clean
main.exe: main.obj so_stdio.dll
	link /out:main.exe main.obj so_stdio.lib

main.obj: main.c
	cl /Fomain.obj /c main.c

so_stdio.dll: so_open.obj so_file.obj so_getc_read.obj so_rw.obj so_proc.obj so_other_functions.obj
	link  /nologo /dll so_open.obj  so_rw.obj so_file.obj so_getc_read.obj so_other_functions.obj	so_proc.obj /out:so_stdio.dll /implib:so_stdio.lib

so_open.obj: so_open.c
	cl /Foso_open.obj /c so_open.c

so_file.obj: so_file.c
	cl /Foso_file.obj /c so_file.c

so_getc_read.obj: so_getc_read.c
	cl /Foso_getc_read.obj /c so_getc_read.c

so_rw.obj: so_rw.c
	cl /Foso_rw.obj /c so_rw.c
so_proc.obj: so_proc.c
	cl /Foso_proc.obj /c so_proc.c
so_other_functions.obj: so_other_functions.c
	cl /Foso_other_functions.obj /c so_other_functions.c
clean:
	del /f *.obj *.dll *.exp *.lib *.exe *.pdb 2>NULL