#include "socket.h"
#include "binary_converter.h"

/*
 * Receptor para protocolo de retroceso-N
 *
 * Autor: Manuel Ignacio Castillo López; 3-0801743-8.
 * manuel_castillo_cc@ciencias.unam.mx
 * Autor: Mijail Gutiérrez Valdéz; 3-423250-3.
 * mij000@ciencias.unam.mx
 * Version 1, marzo 2014
 */

//prototipos de funciones
void parse_args(char *, char *);

//variables globales
float prob;
unsigned short int packlose;
unsigned short int rettime;

/*
 * Instancia un receptor de retroceso-N
 */
int main(int argc, char *argv[])
{
	unsigned short int aux;
	if(argc != 7)
	{
		printf("Debe proporcionar exactamente 6 argumentos al programa.\n");
		printf("Por ejemplo:\n");
		printf("$error_restoring -p 0.01 -P 2 -t 2\n\n");
		printf("Donde:\n");
		printf("\t* -p es la probabilidad de una p\u00E9rdida al recibir");
		printf(" un paquete nuevo y su valor es alguno de los siguientes: ");
		printf("0.01, 0.001, 0.0001\n");
		printf("\t* -P es el n\u00FAmero de paquetes consecutivos perdidos ";
		printf("en el evento de que ocurra una p\u00E9rdida. Puede tener ");
		printf("alguno de los siguientes valores: 1, 2, 3\n");
		printf("\t* -t Es el tiempo de retazo que puede ocurrir con ");
		printf("probabilidad -p. Puede ser alguno de los siguientes: 2, 3");
		printf("; y son multiplos del promedio del tiempo de entrega\n");
	}//verifica los argumentos
	for(aux = 1; aux < 7; aux += 2)
	{
		parse_args(argv[aux], argv[aux +1]);
	}//inicializa el programa conforme a los parámetros de usuario
	int sender = createsocketserver(6660, 5, 0);
	int socket = acceptsocket(sender);
	char *buffer[1024];
	char *aux[512];
	unsigned long long int totreceived = 0; //in a galaxy far far away....
	unsigned short int lossocurred = 0;
	srand(time(NULL));
	double ptime = 0;
	time_t inittime, finaltime;
	struct timespec waiter;
	waiter.tv_sec = 0;
	unsigned short int checksum;
	unsigned int secuence;
	unsigned short int damagedp = 0;
	while(1)
	{
		time(&inittime);
		if(0 > readsocket(socket, buffer, 1024))
		{
			exit(EXIT_FAILURE);
		}//comprueba que no haya errores
		time(&finaltime);
		ptime = (ptime + (finaltime -inittime)) /(totreceived +1);
		if(((rand() %1001) /1000) <= p)
		{
			lossocurred = 1;
			continue;
		}//si se ha cubierto el rango de probabilidad de errores
		if((lossocurred > 0) && (lossocurred < packlose))
		{
			lossocurred++;
			continue;
		}
		else
		{
			lossocurred = 0;
		}//pierde varios paquetes simultáneos
		if(((rand() %1001) /1000) <= p)
		{
			waiter.tv_nsec = (int) (ptime *rettime *100000);
			nanosleep(&waiter, NULL);
		}//si ocurre un retraso
		buffer += 32;
		secuence = bintoi(buffer);
		buffer += 32;
		if(buffer[0] == '1')
		{
			execvp(argv[0], argv);
		}//hace un reset de la conexión
		if(buffer[1] == '1')
		{
			close(socket);
			exit(EXIT_SUCCESS);
		}//si fin era 1
		/*if(buffer[2] == '0')
		{
			printf("LOL!");FIXME LOL!
		}//si SYN era 0*/
		buffer += 20;
		checksum = bintos(buffer);
		if((((secuence +1024) | checksum) != 0) || (((rand() %1001) /1000) <= p))
		{
			printf("Se recibi\u00F3 un paquete da\u00F1ado\n");
			damagedp++;
			continue;
		}//si recibe un paquete dañado
		if((damagedp > 0) && (damagedp < packlose))
		{
			damagedp++;
			continue;
		}
		else
		{
			damagedp = 0;
		}//recibe varios paquetes dañados varios paquetes simultáneos
		itobin(32, totreceived, buffer, 1024);
		itobin(32, totreceived, aux, 512);
		strcat(buffer, aux);
		strcat(buffer, "1001");
		itobin(16, 1024, aux, 512);
		strcat(buffer, aux);
		//el checksum va a ser la suma de totreceived +1024
		itobin(16, (totreceived +1024) *(-1), aux, 512);
		strcat(buffer, aux);
		writesocket(socket, buffer, 1024);
		writesocket(socket, buffer, 1024);
		totreceived++;
		printf("Tiempo promedio de viaje de paquetes: %f\n", ptime);
	}//se comunica con el cliente
	close(socket);
	return 0;
}//main

/*
 * Toma en su primer parámetro una bandera de los parámetros de consola y en el
 * segundo su valor (en caso de requerirlo) para configurar el estado del
 * programa a lo que haya indicado el usuario con los parámetros.
 */
void parse_args(char *arg, char *val)
{
	if(strcmp(arg, "-p") == 0) {
		prob = strtof(val, NULL);
		if(prob != 0.01 && prob != 0.001 && prob != 0.0001)
		{
			printf("-p Debe estar asociado a un flotante con alguno de los ");
			printf("valores siguientes: 0.01, 0.001, 0.0001\n");
			exit(EXIT_FAILURE);
		}//si la probabilidad no es lo que se espera
	} else if(strcmp(arg, "-P") == 0) {
		packlose = atoi(val);
		if(packlose != 1 && packlose != 2 && packlose != 3)
		{
			printf("-P Debe estar asociado a un entero con alguno de los ");
			printf("siguientes valores: 1, 2, 3\n");
			exit(EXIT_FAILURE);
		}//si el número de paquetes perdidos consecutivamente no es esperado
	} else if(strcmp(arg, "-t") == 0) {
		rettime = atoi(val);
		if(rettime != 2 && rettime != 3)
		{
			printf("-t Debe estar asociado a un entero con alguno de los ");
			printf("siguientes valores: 2, 3\n");
			exit(EXIT_FAILURE);
		}//si el tiempo de retraso no tiene alguno de los valores de retraso
	} else {
		printf("No se reconoce el par\u00E1metro %s como uno v\u00E1lido\n",
			arg);
	}//actua dependiendo el parámetro dado
}//parse_args

