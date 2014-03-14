/*
 * Sockets de conexion
 *
 * Autor: Manuel Ignacio Castillo López; 3-0801743-8.
 * manuel_castillo_cc@ciencias.unam.mx
 * Autor: Mijail Gutiérrez Valdéz; 3-423250-3.
 * mij000@ciencias.unam.mx
 * Version 1, marzo 2014
 */

#include "socket.h"

/*
 * Funcion que acepta un cliente del socket servidor sockfd
 * esta regresa el descriptor del socket que se ha aceptado
 */ 
int acceptsocket(int sockfd)
{
	struct sockaddr cli_addr;
	socklen_t clilen;
	int sockfd_cli;

	clilen = sizeof(cli_addr);
	sockfd_cli = accept(sockfd, &cli_addr, &clilen);
	if(sockfd_cli < 0)
	{
		error("Cant accept client");
	}
	return sockfd_cli;
}

/*
 * crea un nuevo socket de servidor en el con el puerto port, con una cola
 * de conexion de num_connect y un tiempo de espera antes de desconectarse 
 * de timeout, si timeout es 0 el socket siempre esta escuchando peticiones
 */
int createsocketserver(int port, int num_connect, int timeout)
{
	struct sockaddr_in serv_addr;
	int sockfd;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
	{
	 	error("Error opening socket");
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(port);
	if (bind (
			sockfd, 
			(struct sockaddr *)&serv_addr, 
			sizeof (serv_addr)) < 0)
	{
		close(sockfd);
		error("Error Binding");
	}
	if(timeout > 0)
	{
		if (setsockopt(
				sockfd,
				SOL_TCP,
				TCP_USER_TIMEOUT,
				(char*) &timeout,
				sizeof (timeout)))
		{
			close(sockfd);
			error("FTP ERROR al ponerle tiempo de espera al socket\n");
		}
	}
	if (listen(sockfd, num_connect) < 0)
	{
		close(sockfd);
		error("Error listing");
	}
	return sockfd;
}

/*
 * Crea un socket cliente con para conectarse a la direccion addr y al
 * puerto port, regresa el discritor de fichero de dicho socket.
 */
int createsocketclient(char *addr, int port)
{
	int sockfd;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		error("ERROR opening socket");
	}
	server = gethostbyname(addr);
	if (server == NULL)
	{
		fprintf(stderr,"ERROR, no such host\n");
		exit(0);
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, 
	(char *)&serv_addr.sin_addr.s_addr,
	server->h_length);
	serv_addr.sin_port = htons(port);
	if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
	{
		error("ERROR connecting");
	}
	return sockfd;
}

/*
 * Lee la informacion mandada a sockfd en el buffer buffer con una cantidad
 * de lectura igual al tamaño del buffer
 */
int readsocket(int sockfd, char *buffer, int size_read)
{
	bzero(buffer, size_read);
	int sread = read (sockfd, buffer, size_read - 1);
	if(sread < 0)
	{
		close(sockfd);
		error("Error no es posible leer del socket");
	}
	return sread;
}

/*
 * escribe informacion al socket sockfd almacenada en el buffer buffer, esta solo
 * escribe la longitud size_write del buffer en el socket
 */
int writesocket(int sockfd, char *buffer, int size_write)
{
	int swrite = write (sockfd, buffer, size_write);
	if(swrite < 0)
	{
		close(sockfd);
		error("Error no es posible escribir en el socket");
	}
	return swrite;
}

/*
 * funcion que lee el archivo apuntado por la ruta path en el socket
 * sockfd creado para tal accion un buffer de tamañp len_buff
 */
void filesockrw(int sockfd, char *path, int len_buff)
{
	FILE *file;
	char buffer[len_buff];
	int size;
	file = fopen(path, "r");
	if (file == NULL)
	{
		printf("FTP No se puede abrir el archivo\n");
		return;
	}
	else
	{
		fseek(file, 0L, SEEK_END);
		size = ftell(file);
		fseek(file, 0L, SEEK_SET);
		bzero(buffer, len_buff);
		sprintf(buffer, "%d", size);
		writesocket(sockfd, buffer, strlen(buffer)); //se escribe el tamaño del archivo
		int send_size = 0;
		while(feof(file) == 0 && send_size < size)
		{
			bzero(buffer, len_buff);
			fgets(buffer, len_buff, file);
			writesocket(sockfd, buffer, strlen(buffer));
			if(readsocket(sockfd, buffer, len_buff) <= 0)
			{
				break;
			}
			send_size += atoi(buffer);
		}
		fclose(file);
	}
}

/*
 * funcion que lee el socket sockfd con un buffer de tamaño len_buff
 * y escribe el resultado en el archivo apuntado por la ruta path.
 */
void sockfilerw(int sockfd, char *path, int len_buff)
{
	FILE *file;
	char buffer[len_buff];
	file = fopen(path, "w");
	if (file == NULL)
	{
		printf("FTP No se puede abrir el archivo\n");
		return;
	}
	else
	{
		readsocket(sockfd, buffer, len_buff);
		int size = atoi(buffer);
		int sread = 0;
		int add = 0;
		while(sread < size)
		{
			readsocket(sockfd, buffer, len_buff);
			add = strlen(buffer);
			sread = sread + add;
			fprintf(file, "%s", buffer);
			bzero(buffer, len_buff);
			sprintf(buffer, "%d", add);
			writesocket(sockfd, buffer, strlen(buffer));
			if(add <= 0)
			{
				break;
			}
		}
		fclose(file);
	}
}

/*
 * crea un puerto aleatorio entre los numero 1024 y 65536
 */
int createport()
{
  int minport=1025;
  int maxport=65536;
  int range = maxport - minport;
  return (rand() % (range+1)) + minport;
}
