#include "socket.h"
#include "binary_converter.h"

/*
 * Receptor para protocolo de repetición selectiva
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
char *window[8];//ventana
unsigned short int current_buffer = 0;
unsigned short int window_size;
short int occuped_windows[8];
unsigned short int data_size = 0;

/*
 * Instancia un receptor de retroceso-N
 */
int main(int argc, char *argv[])
{
	unsigned short int i;
	if(argc != 9)
	{
		printf("Debe proporcionar exactamente 8 argumentos al programa.\n");
		printf("Por ejemplo:\n");
		printf("$error_restoring -p 0.01 -P 2 -t 2 -v 7\n\n");
		printf("Donde:\n");
		printf("\t* -p es la probabilidad de una p\u00E9rdida al recibir");
		printf(" un paquete nuevo y su valor es alguno de los siguientes: ");
		printf("0.01, 0.001, 0.0001\n");
		printf("\t* -P es el n\u00FAmero de paquetes consecutivos perdidos ");
		printf("en el evento de que ocurra una p\u00E9rdida. Puede tener ");
		printf("alguno de los siguientes valores: 1, 2, 3\n");
		printf("\t* -t Es el tiempo de retazo que puede ocurrir con ");
		printf("probabilidad -p. Puede ser alguno de los siguientes: 2, 3");
		printf("; y son multiplos del promedio del tiempo de entrega\n");
		printf("\t* -v Es el número de buffers individuales que componen ");
		printf("una ventana en conjunto. El n\u00FAmero de estos buffers ");
		printf("debe ser alguno de los siguientes: 6, 7, 8\n");
		exit(EXIT_FAILURE);
	}//verifica los argumentos
	for(i = 1; i < 9; i += 2)
	{
		parse_args(argv[i], argv[i +1]);
	}//inicializa el programa conforme a los parámetros de usuario
	for(i = 0; i < window_size; i++)
	{
		window[i] = malloc(sizeof(char *) *1024);
		occuped_windows[i] = -1;
	}//inicializa las ventanas
	int sender = createsocketserver(6660, 5, 0);//se pone satánica la cosa
	int socket = acceptsocket(sender);
	char aux[512];
	unsigned long long int totreceived = 0; //in a galaxy far far away....
	unsigned short int lossocurred = 0;
	srand(time(NULL));
	double ptime = 0;
	time_t inittime, finaltime;
	struct timespec waiter;
	waiter.tv_sec = 0;
	unsigned short int checksum;
	unsigned int secuence;
	unsigned int next_expected;
	unsigned short int damagedp = 0;
	while(1)
	{
		time(&inittime);
		memset(window[current_buffer], 0, 1024);
		if(read(socket, window[current_buffer], 1024) <	 0)
		{
			printf("Error en la lectura del socket\n");
			exit(EXIT_FAILURE);
		}//comprueba que no haya errores
		printf("Recibio: %s\n", window[current_buffer]);
		time(&finaltime);
		ptime = (ptime + (finaltime -inittime)) /(totreceived +1);
		if(((rand() %1001) /1000) <= prob)
		{
			lossocurred = 1;
			writesocket(socket, "", 1024);
			continue;
		}//si se ha cubierto el rango de probabilidad de errores
		if((lossocurred > 0) && (lossocurred < packlose))
		{
			lossocurred++;
			writesocket(socket, "", 1024);
			continue;
		}
		else
		{
			lossocurred = 0;
		}//pierde varios paquetes simultáneos
		if(((rand() %1001) /1000) <= prob)
		{
			waiter.tv_nsec = (int) (ptime *rettime *100000);
			nanosleep(&waiter, NULL);
		}//si ocurre un retraso
		secuence = bintoi(window[current_buffer]);
		window[current_buffer] += 64;
		if(window[current_buffer][0] == '1')
		{
			execvp(argv[0], argv);
		}//hace un reset de la conexión
		if(window[current_buffer][1] == '1')
		{
			close(socket);
			exit(EXIT_SUCCESS);
		}//si fin era 1
		/*if(window[current_buffer][2] == '0')
		{
			printf("LOL!");FIXME LOL!
		}//si SYN era 0*/
		window[current_buffer] += 20;
		checksum = bintos(window[current_buffer]);
		if((((secuence +1024) | checksum) != 0) || (((rand() %1001) /1000) <= prob))
		{
			printf("Se recibi\u00F3 un paquete da\u00F1ado\n");
			damagedp++;
			writesocket(socket, "", 1024);
			continue;
		}//si recibe un paquete dañado
		if((damagedp > 0) && (damagedp < packlose))
		{
			damagedp++;
			writesocket(socket, "", 1024);
			continue;
		}
		else
		{
			damagedp = 0;
		}//recibe varios paquetes dañados varios paquetes simultáneos
		itobin(32, totreceived, window[current_buffer], 1024);
		itobin(32, secuence, aux, 512);
		strcat(window[current_buffer], aux);
		if(secuence == next_expected)
		{
			next_expected++;
			for(i = 0; i < window_size; i++)
			{
				if(occuped_windows[i] == next_expected)
				{
					next_expected++;
					i = 0;
					occuped_windows[i] = -1;
					data_size--;
					continue;
				}//si se encuentra el siguiente en las ventanas lo "sube a la app"
			}//busca en el buffer el siguiente paquete esperado
		}
		else if(data_size < window_size -1)
		{
			occuped_windows[i] = secuence;
			data_size++;
			do
			{
				current_buffer++;
				if(current_buffer >= window_size)
				{
					current_buffer = 0;
				}//si el buffer actual se sale del arreglo
			}//busca el proximo espacio vacio
			while(occuped_windows[current_buffer] != -1);
		}
		else
		{
			writesocket(socket, "", 1024);
			continue;
		}//si la secuencia es la esperada y si es posible almacenarla en otro caso
		strcat(window[current_buffer], "1001");
		itobin(16, 1024, aux, 512);
		strcat(window[current_buffer], aux);
		//el checksum va a ser la suma de totreceived +1024
		itobin(16, (totreceived +1024) *(-1), aux, 512);
		strcat(window[current_buffer], aux);
		writesocket(socket, window[current_buffer], 1024);
		totreceived++;
		printf("Tiempo promedio de viaje de paquetes: %f\n", ptime);
		if(current_buffer < window_size)
		{
			current_buffer++;
		}
		else
		{
			current_buffer = 0;
		}//si el buffer se llena, cicla
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
		if((prob != ((float) 0.01)) && (prob != ((float) 0.001)) && (prob != ((float) 0.0001)))
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
	} else if(strcmp(arg, "-v") == 0) {
		window_size = atoi(val);
		if(window_size != 6 && window_size != 7 && window_size != 8)
		{
			printf("-v Debe estar asociado a un entero con alguno de los ");
			printf("siguientes alores: 6, 7, 8\n");
			exit(EXIT_FAILURE);
		}//si el tamaño de la ventana no era alguno de los esperados
	} else {
		printf("No se reconoce el par\u00E1metro %s como uno v\u00E1lido\n",
			arg);
	}//actua dependiendo el parámetro dado
}//parse_args

