CC = gcc
CFLAGS = -c
LEXERSRC = lexer

all: start clean

start: main.o lexer.o
	$(CC) main.o lexer.o -o start

main.o:
	$(CC) $(CFLAGS) main.c

lexer.o:
	$(CC) $(CFLAGS) $(LEXERSRC)/lexer.c

clean:
	rm -rf *.o
