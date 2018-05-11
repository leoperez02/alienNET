/*
 * alienRx_fun.c
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

extern unsigned char buffer[MTU];
extern int sockfd;
extern int cliente_sockfd;

int get_data(void)
{
	int bytes_readed;
	bytes_readed = read (cliente_sockfd, &buffer, MTU);
	if ( bytes_readed < 0 )
	{
		perror("Error al recibir el mensaje\n");
		exit(1);
	}
	return bytes_readed;
}

char *get_mem(int bytes_readed)
{
	register int i ;
	char* cadena;
	cadena = (char*) malloc(sizeof(unsigned char)*bytes_readed); 
	for( i = 0 ; i < bytes_readed ; i++)
	{
		cadena[i] = buffer[i];
	}
	return cadena;
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

void gotoxy(int x, int y)
{
	printf("%c[%d;%df",0x1B,y,x);
}

void isr_signal(int signal)
{
	int status;
	pid_t pid;
	if ( signal == SIGUSR1 )
	{
		printf("Señal SIGUSR1 recibida\n");
		/**
		 * Esperando al proceso hijo para finalizar
		 * */
		pid = wait(&status);
		status>>=8;
		printf("Proceso %d terminado con resultado %d\n",pid,status); 
	}
}

