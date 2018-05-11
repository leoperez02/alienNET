/*
 * alienRx.c
 * 
 * Copyright 2018 Leo <leo@Inspiron3437>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

#include "alienRx_fun.h"

int sockfd, cliente_sockfd;
unsigned char buffer[MTU];

void read_client(void);

void start_server(void)
{
   	//Estructura de la familia AF_INET, que almacena direccion
   	struct sockaddr_in direccion_servidor; 
   	
   	/**
   	 * Colocando señal SIGUSR1 para escuchar por el procso padre
   	 * */
   	if ( signal( SIGUSR1, isr_signal) == SIG_ERR ) 
	{
		perror("Error en señal\n");
		exit(EXIT_FAILURE);
	}
	
	/*	
	 *	AF_INET - Protocolo de internet IPV4
	 *  htons - El ordenamiento de bytes en la red es siempre big-endian, 
	 * 	por lo que en arquitecturas little-endian se deben revertir los 
	 * 	bytes
	 *  INADDR_ANY - Se elige cualquier interfaz de entrada
	 */	
	//se limpia la estructura con ceros
   	memset (&direccion_servidor, 0, sizeof (direccion_servidor));	
   	direccion_servidor.sin_family 		= AF_INET;
   	direccion_servidor.sin_port 		= htons(PUERTO);
   	direccion_servidor.sin_addr.s_addr 	= INADDR_ANY;

	/*
	 *	Creacion de las estructuras necesarias para el manejo de un 
	 * 	socket
	 *  SOCK_STREAM - Protocolo orientado a conexión
	 */
	printf("Creando Socket ....\n");
	if( (sockfd = socket (AF_INET, SOCK_STREAM, 0)) < 0 )
	{
		perror("Ocurrio un problema en la creacion del socket");
		exit(1);
	}
	/*
	 *  bind - Se utiliza para unir un socket con una dirección de red
	 *  determinada
	 */
	printf("Configurando socket ...\n");
	if( bind(sockfd, (struct sockaddr *) &direccion_servidor, sizeof(direccion_servidor)) < 0 )
	{
		perror ("Ocurrio un problema al configurar el socket");
		exit(1);
	}
	/*
	 *  listen - Marca al socket indicado por sockfd como un socket 
	 * 	pasivo, esto es, como un socket que será usado para aceptar 
	 * 	solicitudes de conexiones de entrada usando "accept"
	 *  Habilita una cola asociada la socket para alojar peticiones de 
	 * 	conector procedentes de los procesos clientes
	 */
	printf ("Estableciendo la aceptacion de clientes...\n");
	if( listen(sockfd, COLA_CLIENTES) < 0 )
	{
		perror("Ocurrio un problema al crear la cola de aceptar peticiones de los clientes");
		exit(1);
	}
	/*
	 *  accept - Aceptación de una conexión
	 *  Selecciona un cliente de la cola de conexiones establecidas
	 *  se crea un nuevo descriptor de socket para el manejo
	 *  de la nueva conexion. Apartir de este punto se debe
	 *  utilizar el nuevo descriptor de socket
	 *  accept - ES BLOQUEANTE 
	 */

	while(1)
	{
		printf ("En espera de peticiones de conexión ...\n");
		if( (cliente_sockfd = accept(sockfd, NULL, NULL)) < 0 )
		{
			perror("Ocurrio algun problema al atender a un cliente");
			exit(1);
		}
	
		pid_t pid = fork();
		if( !pid ) /* proceso hijo*/
		{
			printf("Se acepto un cliente, atendiendolo... \n");
			read_client();
		}
		printf("Concluimos la ejecución de la aplicacion Servidor \n");
	}
	/*
	 *	Cierre de las conexiones
	 */
	close (sockfd);
}

void read_client(void)
{
	/*
	* Inicia la transferencia de datos entre cliente y servidor
	*/
	FILE *file;
	int bytes_readed, bytes_writed, bytes;
	char *file_name, *ruta_destino, *path_file;
	/**
	 * Recibir archivo
	 * 
	 * 1. Recibir ruta donde guardar el archivo con nombre
	 * 2. Recibir contenido del archivo
	 * */
	
	/**
	 * 1. Recibir nombre del archivo
	 * */
	printf("Esperando ruta y nombre destino... ");			
	bytes_readed = get_data();
	printf("leidos %d bytes del stream\n",bytes_readed);
	/**
	 * Reservar memoria para la cadena del nombre de archivo
	 * y colocar el contenido del buffer en la memoria
	 * */
	path_file = get_mem(bytes_readed);
	printf("Destino recibido: %s\n",path_file);
	/**
	 * Enviar de vuelta el nombre para confirmar
	 * */
	bytes_writed = write (cliente_sockfd, path_file, strlen(path_file)); 
	if ( bytes_writed != strlen(path_file))
	{
		perror("No se escribio correctamente el nombre de archivo\n");
		exit(1);
	}
	
	/**
	 * 3. Recibiendo los bytes del archivo
	 * */ 
	//path_file = get_path_file(ruta_destino,file_name);
	printf("Creando archivo %s\n",path_file);
	/**
	* Crear archivo binario para escritura
	* */
	file = fopen(path_file,"wb");
	/**
	* Verificar que no hay error en la apertura
	* */
	if(!file)
	{
		perror ("Ocurrio un problema al abrir el archivo\n");
		exit(1);
	}
	/**
	 * Recibir bytes ...
	 * */
	printf("Recibiendo archivo ...\n");
	
	bytes=0;
	bytes_readed = 0;
	bytes_writed = 0 ;
	
	bytes_readed = read (cliente_sockfd, &buffer, MTU); 
	while( bytes_readed )
	{	
		if( bytes_readed < 0 )
		{
			perror ("Ocurrio algun problema al recibir datos del cliente");
			exit(1);
		}
		bytes_writed = fwrite(&buffer, sizeof(unsigned char), bytes_readed, file);
		if( bytes_writed < 0 )
		{
			perror("Error al leer el stream de datos\n");
		}
		bytes+=bytes_readed;
		printf("Recibidos %d bytes del archivo\n",bytes);
		bytes_readed = read (cliente_sockfd, &buffer, MTU);
		gotoxy(1,12);
	}
	gotoxy(1,13);
	//	Cerrar descriptor de archivo
	fclose(file);
	
	/**
	 * Para finalizar correctamente el proceso hijo, debe 
	 * enviarse una señal al proceso padre que avise que el 
	 * proceso hijo ha finalizado.
	 * 
	 * La señal funciona como una interrupcion en el proceso 
	 * padre para que no hacer el proceso bloqueante 
	 * */
	close (cliente_sockfd); // Cerrar socket cliente
	kill( getppid(), SIGUSR1 );
	exit(EXIT_SUCCESS);
}
