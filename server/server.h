#ifndef SERVER_H
#define SERVER_H

#define MAX_PENDING 1
#define BUFFER_SIZE 512
#define MAX_CLIENTS 2

#define SELECT_TIMEOUT_SEC 1
#define SELECT_TIMEOUT_MICROSEC 0

#define OUTPUT_CHANNEL stdout
#define INPUT_CHANNEL stdin
#define ERROR_CHANNEL stderr

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <netinet/in.h>
#include "../util/connection.h"

typedef struct {
    fd_set *read_set;
    int *client_connections;
    int num_connected;
} server_data_t;

int run_server();

int poll_clients(server_data_t *server_data);

int setup_server_socket();

void cleanup_server(server_data_t *server_data, int server_socket);

int get_available_sockets(fd_set *sockets, int *monitored_descriptors, size_t num_monitored);

int handle_client_connections(int socket_fd);

int read_msg_from_client(int fd_to_read, int client_id);




#endif
