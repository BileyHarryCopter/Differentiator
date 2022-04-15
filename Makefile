CC = gcc
CFLAGS = -c
LEXERSRC = lexer
PARCERSRC = parcer
SERVICESRC = service

all: start clean

start: main.o lexer.o parcer.o service.o
	$(CC) main.o lexer.o parcer.o service.o -o start

main.o:
	$(CC) $(CFLAGS) main.c

lexer.o:
	$(CC) $(CFLAGS) $(LEXERSRC)/lexer.c

parcer.o:
	$(CC) $(CFLAGS) $(PARCERSRC)/parcer.c

service.o:
	$(CC) $(CFLAGS) $(SERVICESRC)/service.c

clean:
	rm -rf *.o

leaks:
	leaks -atExit -- ./start
