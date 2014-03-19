# environment variables. Compiler and its flags
# to build a debuggable version call make as: "make G=-g"
# this also can be used to pass custom flags to gcc to build the progam
# be aware that the G flags will also be present on executable builds, so be wise when using it.
CC = gcc
CFLAGS = -c -Wall -x c -I ./include/ -O2 -ffunction-sections -fdata-sections -funit-at-a-time -std=c99 -D_GNU_SOURCE

all : build

build : bin.o n_ret.o sock.o err.o rep.o
	$(CC) $(G) lib/dectobin.o lib/error.o lib/sock.o lib/n_ret.o -o bin/n_retroceso
	$(CC) $(G) lib/dectobin.o lib/error.o lib/sock.o lib/rep.o -o bin/repeticion_selectiva
	chmod 774 bin/error_restoring

bin.o :
	$(CC) $(CFLAGS) $(G) src/binary_converter.c -o lib/dectobin.o

n_ret.o :
	$(CC) $(CFLAGS) $(G) src/n_retroceso.c -o lib/n_ret.o

sock.o :
	$(CC) $(CFLAGS) $(G) src/socket.c -o lib/sock.o

err.o :
	$(CC) $(CFLAGS) $(G) src/error.c -o lib/error.o

rep.o :
	$(CC) $(CFLAGS) $(G) src/rep_select.c -o lib/rep.o

clean :
	rm -f bin/*

cleanall :
	rm -f bin/* lib/*


