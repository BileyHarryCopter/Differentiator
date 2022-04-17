CC = gcc
CFLAGS = -c
LEXERSRC = lexer
PARCERSRC = parcer
SERVICESRC = service
DUMPSRC = dump
DIFFSRC = difftorr

all: start clean

start: main.o lexer.o parcer.o service.o dump.o difftorr.o
	$(CC) main.o lexer.o parcer.o service.o dump.o difftorr.o -o start

main.o:
	$(CC) $(CFLAGS) main.c

lexer.o:
	$(CC) $(CFLAGS) $(LEXERSRC)/lexer.c

parcer.o:
	$(CC) $(CFLAGS) $(PARCERSRC)/parcer.c

service.o:
	$(CC) $(CFLAGS) $(SERVICESRC)/service.c

dump.o:
	$(CC) $(CFLAGS) $(DUMPSRC)/dump.c

difftorr.o:
	$(CC) $(CFLAGS) $(DIFFSRC)/difftorr.c

clean:
	rm -rf *.o

leaks:
	leaks -atExit -- ./start
