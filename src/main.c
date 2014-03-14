#include "socket.h"

/*
 * Receptor para protocolo de retroceso-N
 *
 * Autor: Manuel Ignacio Castillo López; 3-0801743-8.
 * manuel_castillo_cc@ciencias.unam.mx
 * Autor: Mijail Gutiérrez Valdéz; 3-423250-3.
 * mij000@ciencias.unam.mx
 * Version 1, marzo 2014
 */

/*
 * Instancia un receptor de retroceso-N
 */
int main(int argc, char *argv[])
{
	int sender = createsocketserver(6660, 5, 0);
	int socket = acceptsocket(sender);
	char *buffer = "Hola";
	writesocket(socket, buffer, 4);
	close(socket);
	return 0;
}//main

