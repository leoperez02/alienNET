#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "alienTx_defs.h"

int get_data(void);
char* get_file_name(char *file_name);
void get_menu(void);
void gotoxy(int x,int y);
int send_data(char *data);
void syntax_error(void);
void transfer_file(char *src_file, char *ip_address, char *dest_path);
