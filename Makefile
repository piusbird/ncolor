PREFIX=/usr/local
CC=gcc
CFLAGS=-Wall -Werror 
all: clean  ncolor
ncolor: ncolor.c
	$(CC) $(CFLAGS) -o ncolor ncolor.c -I.

clean: 
	rm -f ncolor
indent:
	indent -linux ncolor.c
	rm -f *~

install: ncolor

	install -m755 ncolor $(PREFIX)/bin/ncolor
	
