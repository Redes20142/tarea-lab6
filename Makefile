# environment variables. Compiler and its flags
# to build a debuggable version call make as: "make G=-g"
# this also can be used to pass custom flags to gcc to build the progam
# be aware that the G flags will also be present on executable builds, so be wise when using it.
CC = gcc
CFLAGS = -c -Wall -x c -I ./include/ -O2 -ffunction-sections -fdata-sections -funit-at-a-time -std=c99 -D_GNU_SOURCE

all : build

build : err.o sock.o main.o
	$(CC) $(G) lib/error.o lib/sock.o lib/main.o -o bin/tarea-lab6
	chmod 774 bin/tarea-lab6

main.o :
	$(CC) $(CFLAGS) $(G) src/main.c -o lib/main.o

sock.o :
	$(CC) $(CFLAGS) $(G) src/socket.c -o lib/sock.o

err.o :
	$(CC) $(CFLAGS) $(G) src/error.c -o lib/error.o

clean :
	rm -f bin/*

cleanall :
	rm -f bin/* lib/*

exec :
	bin/tarea-lab6

