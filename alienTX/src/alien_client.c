#include "alienTx_fun.h"

void get_menu();
/**
 * La ejecución del programa tiene la siguiente sintaxis
 * 
 * 1. Para transfrir el archivo FILE1 al servidor con la dirección IP y 
 * escuchando en el PUERTO, en la ruta RUTA_DESTINO 
 * $ ./clienteTCP FILE1 IP PUERTO RUTA_DESTINO 
 * 
 * 2. Para transferir todos los archivos de un directorio 
 * (no es recursivo)
 * $ ./clienteTCP RUTA_DIR IP PUERTO RUTA_DESTINO 
 * 
 * */
int main(int argc, char **argv)
{
	/**
	 * Validar argumentos recibidos al ejecutar el programa
	 * 1. nombre del archivo
	 * 2. direccion IP
	 * 3. Puerto 
	 * 4. ruta destino (debe incluir la diagonal final)
	 * 	ejemplo: /home/user/dir1/dir2/
	 * */
	system("clear");	
	if ( argc < 5)
	{
		perror("Hacen falta argumentos para ejecutar la transferencia\n");
		get_menu();
		//gotoxy(10,6);
		//printf(" hello world \n"); 
		exit(1);
	}
	char *file_name = argv[1];
	char *DIR_IP = argv[2];
	char *PUERTO = argv[3];
	char *ruta_destino = argv[4];
	
	transfer(file_name, DIR_IP, PUERTO, ruta_destino);
	
	return 0;	
}

void get_menu()
{
	printf(" * La ejecución del programa tiene la siguiente sintaxis\n"					);
	printf(" *\n" 						);
	printf(" * 1. Para transfrir el archivo FILE1 al servidor con la dirección IP y \n");
	printf(" * escuchando en el PUERTO, en la ruta RUTA_DESTINO\n" );
	printf(" * $ ./clienteTCP FILE1 IP PUERTO RUTA_DESTINO\n" );
	printf(" * \n");
	printf(" * 2. Para transferir todos los archivos de un directorio \n");
	printf(" * (no es recursivo\n");
	printf(" * $ ./clienteTCP RUTA_DIR IP PUERTO RUTA_DESTINO \n");
}
