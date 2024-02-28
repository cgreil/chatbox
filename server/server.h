#ifndef SERVER_H
#define SERVER_H

#define MAX_PENDING 1
#define BUFFER_SIZE 512

#define MAX_CLIENTS 2

#define OUTPUT_CHANNEL stdout
#define INPUT_CHANNEL stdin
#define ERROR_CHANNEL stderr

#include "../util/connection.h"

void run_server(connection_t connection);

int setup_server_socket();

void cleanup_server(int *client_fd, int num_connected, int socket_fd);




#endif