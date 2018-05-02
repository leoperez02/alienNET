#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "alienTx_defs.h"

int get_data();
int send_data(char *data);
void transfer(char *file_name, char *DIR_IP, char *PUERTO, char *ruta_destino);
char *get_file_name(char *file_name);
void gotoxy(int x,int y);
