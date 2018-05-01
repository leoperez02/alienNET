#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "alienRx_defs.h"


void start_server(void);
void isr_signal(int signal);
int get_data();
char *get_mem(int bytes_readed);
char *get_path_file(char *ruta_destino, char *file_name);

