#include "alienTx_fun.h"

extern int  sockfd;
extern unsigned char buffer[MAX_BUF];

int	get_data()
{
	int bytes_readed;
	bytes_readed = read(sockfd, &buffer, MAX_BUF);
	if( bytes_readed < 0 )
	{
		perror("Ocurrio un problema al RECIBIR el mensaje\n");
		exit(1);
	}
	return bytes_readed;
}

char* get_file_name(char *file_name)
{
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
	return file_name;
	//printf("\nNOMBRE DEL ARCHIVO: %s\n",file_name);
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

void gotoxy(int x,int y)
{
	printf("%c[%d;%df",0x1B,y,x);
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
