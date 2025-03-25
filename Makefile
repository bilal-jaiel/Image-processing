CC = gcc
CFLAGS = -Wall -Wextra
EXEC = projet

all : prog

filenam.o : filename.c 
	$(CC) $(CFLAGS) -c $^

pictures.o : pictures.c 
	$(CC) $(CFLAGS) -c $^

pixels.o : pixels.c 
	$(CC) $(CFLAGS) -c $^

lut.o : lut.c 
	$(CC) $(CFLAGS) -c $^

main.o : main.c
	$(CC) $(CFLAGS) -c $^

prog : filename.o pictures.o pixels.o lut.o main.o
	$(CC) $(CFLAGS) -o $(EXEC) $^