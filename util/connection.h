#ifndef CONNECTION_H
#define CONNECTION_H

#define SOCKET_PATH "./socket"


typedef struct connection {
    int connection_fd;
} connection_t;


int create_connection_pool(connection_t **connection);

int clean_connections();








#endif