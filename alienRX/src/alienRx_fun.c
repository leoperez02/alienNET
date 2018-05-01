#include "alienRx_fun.h"

extern unsigned char buffer[TAM_BUFFER];
extern int sockfd;
extern int cliente_sockfd;

void isr_signal(int signal)
{
	int re;
	pid_t pid;
	if ( signal == SIGUSR1 )
	{
		printf("Señal SIGUSR1 recibida\n");
		/**
		 * Esperando al proceso hijo para finalizar
		 * */
		pid = wait(&re);
		printf("Proceso hijo terminado, con resultado %d y PID %d\n",re>>8,pid); 
	}
}

int get_data()
{
	int bytes_readed;
	bytes_readed = read (cliente_sockfd, &buffer, TAM_BUFFER);
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
