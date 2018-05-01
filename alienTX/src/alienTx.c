#include "alienTx_fun.h"

int  sockfd;
unsigned char buffer[MAX_BUF];

void transfer(char *path_file, char *DIR_IP, char *PUERTO, char *ruta_destino)
{	
	struct sockaddr_in direccion_servidor;
	
	//printf("Ruta de dstino %s\n", ruta_destino);

/*	
 *	AF_INET - Protocolo de internet IPV4
 *  htons - El ordenamiento de bytes en la red es siempre big-endian, 
 * 	por lo que en arquitecturas little-endian se deben revertir los bytes
 */	
	/**
	 * La zona de memoria apuntada por &direccion_servidor se llena con
	 * ceros
	 * */
	memset (&direccion_servidor, 0, sizeof (direccion_servidor));
	/**
	 * Se establece como dominio de comunicacion del socket la familia
	 * AF_INET 
	 * */
	direccion_servidor.sin_family = AF_INET;
	/**
	 * Asignacion de puerto de conexion del socket
	 * */
	direccion_servidor.sin_port = htons( atoi(PUERTO) );
/*	
 *	inet_pton - Convierte direcciones de texto IPv4 en forma binaria
 */	
	if( inet_pton(AF_INET, DIR_IP, &direccion_servidor.sin_addr) <= 0 )
	{
		perror("Ocurrio un error al momento de asignar la direccion IP");
		exit(1);
	}
/*
 *	Creacion de las estructuras necesarias para el manejo de un socket
 *  SOCK_STREAM - Protocolo orientado a conexión
 */
	printf("Creando Socket ....\n");
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
	printf ("Estableciendo conexion ...\n");
	if( connect(sockfd, (struct sockaddr *)&direccion_servidor, sizeof(direccion_servidor) ) < 0) 
	{
		perror ("Ocurrio un problema al establecer la conexion");
		exit(1);
	}
/*
 *	Inicia la transferencia de datos entre cliente y servidor
 */	
	/**
	 * Enviar archivo
	 * 
	 * 1. Enviar nombre del archivo
	 * 2. Enviar ruta donde guardar el archivo
	 * 3. Enviar contenido del archivo
	 * */
	 
	FILE *file;
	int bytes, bytes_writed, bytes_readed;
	char *file_name;
	/**
	 * La cadena del nombre del archivo puede incluir la ruta.
	 * Es necesario inspeccionar la cadena leida para determinar si se
	 * tiene el nombre del archivo solamente o esta incluida la ruta.
	 * 
	 * En caso de estar incluida la ruta, se tranferira como nombre de 
	 * archivo, los ultimos caracteres encontrado a partir del ultimo '/'
	 * leido en la cadena
	*/
	
	/**
	 * 1. Enviar nombre del archivo
	 * */
	file_name = get_file_name(path_file); 
	printf("Transfiriendo nombre de archivo destino: %s\n",file_name);
	bytes_writed = send_data(file_name);
	
	/**
	 * Esperar respuesta del servidor, regresa el nombre de archivo 
	 * recibido
	 * */
	bytes_readed = get_data();
	
	if( bytes_readed != bytes_writed )
	{
		perror("Fallo la transmision del nombre de archivo\n");
		exit(1);
	}
	
	//se omite comprobacion buffer == file_name
	
	/**
	 * 2. Enviar ruta de destino
	 * */
	printf("Transfiriendo ruta de archivo destino: %s\n",ruta_destino);
	bytes_writed = send_data(ruta_destino);

	/**
	 * Esperar respuesta del servidor, regresa la ruta de destino 
	 * */
	bytes_readed = get_data();
	
	if( bytes_readed != bytes_writed )
	{
		perror("Fallo la transmision de la ruta de destino\n");
		exit(1);
	}
	
	//se omite comprobacion buffer == ruta_destino
	
	/**
	 * 3. Enviar bytes del archivo
	 * */
 
	/**
	 * Abrir archivo a transferir en modo binario de lectura
	 * */
	file = fopen(path_file,"rb");
		
	/**
	 * Verificar que no hay error en la apertura
	 * */
	if(!file)
	{
		perror ("Ocurrio un problema al abrir el archivo\n");
		exit(1);
	}
	
	/**
	 * Transferir bytes ...
	 * */
	printf("Iniciando transferencia...\n");

	bytes=0; /** Contador de los bytes transferidos */
	
	bytes_readed = fread(&buffer, sizeof(unsigned char), MAX_BUF, file);
	while ( bytes_readed )
	{
		bytes_writed = write(sockfd, &buffer, bytes_readed); 
		if( bytes_writed < 0 )
		{
			perror("Ocurrio un problema en el envio del mensaje\n");
			exit(1);
		}
		bytes+=bytes_writed;
		printf("Enviados %d bytes del archivo ...\n",bytes);
		bytes_readed = fread(&buffer, sizeof(unsigned char), MAX_BUF, file);
	}
	fclose(file);
	
	printf ("Archivo enviado ...\n");
	printf ("Cerrando la aplicacion cliente\n");
/*
 *	Cierre de la conexion
 */
	close(sockfd);
}
