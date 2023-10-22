#ifndef CLIENT_H
#define CLIENT_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

#define BUFFER_SIZE 512

#define READ_END 0
#define WRITE_END 1


void prepare_client(const int *client_to_server, const int *server_to_client);

void send_message_to_server(const int *client_to_server, char *message);






#endif