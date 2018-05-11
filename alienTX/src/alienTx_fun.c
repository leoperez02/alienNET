/*
 * alienTx_fun.c
 * 
 * Copyright 2018 Lain Iwakura <lain@pavilion>
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

#include "alienTx_fun.h"

extern int sockfd;
extern unsigned char buffer[MTU];

int	get_data(void)
{
	int bytes_readed;
	bytes_readed = read(sockfd, &buffer, MTU);
	if( bytes_readed < 0 )
	{
		perror("Ocurrio un problema al RECIBIR el mensaje\n");
		exit(1);
	}
	return bytes_readed;
}

char *get_file_name(char *file_name)
{
	/**
	 * Caso 1: 
	 * La cadena contiene nombre del archivo puede incluir la ruta.
	 * Es necesario inspeccionar la cadena leida para determinar si se
	 * tiene el nombre del archivo solamente o esta incluida la ruta.
	 * 
	 * En caso de estar incluida la ruta, se tranferira como nombre de 
	 * archivo, los ultimos caracteres encontrados a partir de la ultima
	 * diagonal '/' leida en la cadena
	*/
	
	register int i ;
	int rcount, len;
	char aux, *ptr;
	ptr = file_name;
	len = strlen(file_name);
	rcount = len - 1;
	aux = file_name[rcount];
	while ( aux != '/' && rcount > -1 )
	{
		rcount--;
		aux = file_name[rcount];
	}
	if ( rcount <= -1 )
	{
		file_name = (char*) malloc (sizeof(char) * len );
		file_name = ptr;
		return file_name;
	}
	file_name = (char*) malloc (sizeof(char) * (len - rcount) );
	for( i = 0 ; i < (len - rcount) ; i++)
	{
		file_name[i] = ptr[rcount+i+1];
	}
	//printf("\nNOMBRE DEL ARCHIVO: %s\n",file_name);
	return file_name;
}	

void get_menu(void)
{
	printf(" * Sintaxis del programa\n"					);
	printf(" *\n" 						);
	printf(" * 1:\n");
	printf(" * alien_client FILE IP_ADDRESS [DEST_PATH[DEST_FILE]]\n" );
	printf(" * \n");
	printf(" * 2:\n");
	printf(" * alien_client PATH_DIR IP_ADDRESS [DEST_PATH]\n");
}

char *get_path_file(char *ruta_destino, char *file_name)
{
	char *path;
	int len;
	register int i, j ;
	len = strlen(ruta_destino) + strlen(file_name);
	//printf("len = %d",len);
	path = (char*) malloc(sizeof(char)*len );
	for( i = 0 ; i < strlen(ruta_destino) ; i++)
	{
		path[i] = ruta_destino[i];
	}
	for(j = 0 ; j < strlen(file_name) ; i++, j++)
	{
		path[i] = file_name[j];
	}
	return path;
}

void gotoxy(int x,int y)
{
	printf("%c[%d;%df",0x1B,y,x);
}

void make_socket(char* ip_address)
{
	/*	
	 *	Estructura para una direccion IP de un socket 
	 */
	struct sockaddr_in direccion_servidor;	
	/**
	 * La zona de memoria apuntada por &direccion_servidor se llena con
	 * ceros
	 * */
	memset (&direccion_servidor, 0, sizeof (direccion_servidor));
	/**
	 * Se establece como dominio de comunicacion del socket la familia
	 * AF_INET - Protocolo de internet IPv4
	 * */
	direccion_servidor.sin_family = AF_INET;
	/**
	 * Asignacion de puerto de conexion del socket
	 * */
	/*
	 * htons() El ordenamiento de bytes en la red es siempre big-endian, 
	 * por lo que en arquitecturas little-endian se deben revertir los 
	 * bytes
	 * */
	direccion_servidor.sin_port = htons( PUERTO );
	/*	
	 *	inet_pton - Convierte direcciones de texto IPv4 en forma binaria
	*/	
	if( inet_pton(AF_INET, ip_address, &direccion_servidor.sin_addr) <= 0 )
	{
		perror("Ocurrio un error al momento de asignar la direccion IP");
		exit(1);
	}
	/*
	 *	Creacion de las estructuras necesarias para el manejo de un 
	 *  socket
	 *  SOCK_STREAM - Protocolo orientado a conexión
	 */
	//printf("Creando Socket ....\n");
	if( (sockfd = socket (AF_INET, SOCK_STREAM, 0)) < 0 )
	{
		perror("Ocurrio un problema en la creacion del socket");
		exit(1);
	}
	/*
	 *	Inicia el establecimiento de una conexion mediante una apertura
	 *	activa con el servidor
	 *  connect - ES BLOQUEANTE
	 */
	//printf ("Estableciendo conexion ...\n");
	if( connect(sockfd, (struct sockaddr *)&direccion_servidor, sizeof(direccion_servidor) ) < 0) 
	{
		perror ("Ocurrio un problema al establecer la conexion");
		exit(1);
	}
}

void psyduck_warning(void)
{
	printf("\n***************** ADVERTENCIA *****************\n");
	printf("\n* Un psyduck salvaje esta a punto de aparecer *");
	printf("\n\n***********************************************\n");
}

int send_data(char *data)
{
	int len_data, bytes_writed;
	len_data = strlen(data);
	bytes_writed = write( sockfd, data, len_data ); 
	if( bytes_writed < 0 )
	{
		perror("Ocurrio un problema al ENVIAR el mensaje\n");
		exit(1);
	}
	return bytes_writed;
}

void syntax_error(void)
{
	perror("ERROR: Faltan argumentos para ejecutar el programa\n");
	get_menu();
	exit(1);
}
