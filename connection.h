#ifndef CONNECTION_H
#define CONNECTION_H

#define PIPE_SIZE 2

typedef struct connection {
    int client_to_server[PIPE_SIZE];
    int server_to_client[PIPE_SIZE];
} connection_t;


#endif