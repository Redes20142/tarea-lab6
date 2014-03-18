# environment variables. Compiler and its flags
# to build a debuggable version call make as: "make G=-g"
# this also can be used to pass custom flags to gcc to build the progam
# be aware that the G flags will also be present on executable builds, so be wise when using it.
CC = gcc
CFLAGS = -c -Wall -x c -I ./include/ -O2 -ffunction-sections -fdata-sections -funit-at-a-time -std=c99 -D_GNU_SOURCE

all : build

build : bin.o erres.o sock.o err.o
	$(CC) $(G) lib/dectobin.o lib/error.o lib/sock.o lib/erres.o -o bin/error_restoring
	chmod 774 bin/error_restoring

bin.o :
	$(CC) $(CFLAGS) $(G) src/binary_converter.c -o lib/dectobin.o

erres.o :
	$(CC) $(CFLAGS) $(G) src/err_restoring.c -o lib/err_restoring.o

sock.o :
	$(CC) $(CFLAGS) $(G) src/socket.c -o lib/sock.o

err.o :
	$(CC) $(CFLAGS) $(G) src/error.c -o lib/error.o

clean :
	rm -f bin/*

cleanall :
	rm -f bin/* lib/*


