CC=gcc
CFLAGS=-g -Wall

shell: main.o shell.o
	$(CC) $(CFLAGS) -o shell main.o shell.o
main.o: main.c main.h 
	$(CC) $(CFLAGS) -c main.c
shell.o: shell.c shell.h 
	$(CC) $(CFLAGS) -c shell.c

