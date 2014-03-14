/*
 * Cabecera de sockets de conexion
 *
 * Autor: Manuel Ignacio Castillo López; 3-0801743-8.
 * manuel_castillo_cc@ciencias.unam.mx
 * Autor: Mijail Gutiérrez Valdéz; 3-423250-3.
 * mij000@ciencias.unam.mx
 * Version 1, marzo 2014
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <time.h> 
#include <stdio.h>
#include <stdlib.h>
#include "error.h"

//se definen las constantes
#define SOL_TCP 6
#define TCP_USER_TIMEOUT 18

//protopipos
int createsocketserver(int, int, int );
int createsocketclient(char *, int);
int acceptsocket(int );
int readsocket (int, char *, int );
int writesocket (int, char *, int);
void filesockrw (int, char *, int);
void sockfilerw (int, char *, int);
int createport();

