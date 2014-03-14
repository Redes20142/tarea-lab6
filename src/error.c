/*
 * Archivo para mostrar errores
 *
 * Autor: Manuel Ignacio Castillo López; 3-0801743-8.
 * manuel_castillo_cc@ciencias.unam.mx
 * Autor: Mijail Gutiérrez Valdéz; 3-423250-3.
 * mij000@ciencias.unam.mx
 * Version 1, marzo 2014
 */

//se inclye la cabecera
#include "error.h"

/*
 * funcion para lanzar el perror del msg una vez 
 * ejecutado perror sale de simstema con un exit_failure.
 */ 
void error(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}