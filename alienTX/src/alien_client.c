/*
 * alien_client.c
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

/**
 * El programa cliente puede ejecutarse en dos modos:
 * 1. Transferencia de un solo archivo
 * 2. Transferencia de los archivos en un directorio (no es recursivo)
 * 
 * La sintaxis del programa es la siguiente:
 * 
 * 1:
 * alien_client FILE IP_ADDRESS [DEST_PATH[DEST_FILE]]
 * 
 * Se transfiere el archivo FILE al servidor con la direccion IP_ADDRESS
 * en la ubicacion default del servidor a menos que se indique la ruta
 * alternativa DEST_PATH y con mismo nombre de archivo a menos que se
 * incluya el nuevo nombre DEST_FILE para el archivo
 * 
 * 
 * 2:
 * alien_client PATH_DIR IP_ADDRESS [DEST_PATH]
 * 
 * Se transfieren todos los archivos del directorio PATH_DIR al servidor
 * con la direccion IP_ADDRESS en la ubicacion default a menos que se
 * indique la ruta alternativa DEST_PATH
 * 
 * */

int main(int argc, char *argv[])
{
	/*
	 * Validar argumentos recibidos al ejecutar el programa
	 * 1. Ruta y nombre del archivo o ruta del directorio 
	 * 2. Direccion IP del servidor
	 * 4. Ruta destino o ruta destino y nombre
	 *
	 * NOTA: las rutas de directorios deben finalizar con diagonal '/'
	 * */
	
	char *src_file, *ip_address, *dest_path;
	system(CLEAR_SCREEN);	
	switch (argc)
	{
		case 1: // Faltan argumentos para ejecutar el programa
			syntax_error();
		break;
		
		case 2: // Faltan argumentos para ejecutar el programa
			syntax_error();
		break;
		
		case 3: // Enviar un archivo o directorio a la ruta default
			src_file = argv[1];
			ip_address = argv[2];
			transfer_file(src_file, ip_address, NULL);
		break;

		case 4: // Enviar un archivo o directorio a la ruta indicada
			src_file = argv[1];
			ip_address = argv[2];
			dest_path = argv[3];
			transfer_file(src_file, ip_address, dest_path);
		break;
		
		case 5:
			psyduck_warning();
		break;
		
		default:
			syntax_error();
		break;
	}	
	return 0;	
}
