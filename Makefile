all: clean indent ncolor
ncolor: ncolor.c
	gcc -o ncolor ncolor.c -I.

clean: 
	rm -f ncolor
indent:
	indent -linux ncolor.c
	rm -f *~

	
