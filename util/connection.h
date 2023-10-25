#ifndef CONNECTION_H
#define CONNECTION_H


typedef struct connection {
    int **client_to_server;
    int **server_to_client;
} connection_t;


#endif