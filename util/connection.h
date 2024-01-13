#ifndef CONNECTION_H
#define CONNECTION_H

#define MAX_CONNECTIONS 9

#define SOCKET_PATH "./socket"


#define STC_CONNECTION_PREFIX "/tmp/STC_"
#define CTS_CONNECTION_PREFIX "/tmp/CTS_"

typedef struct connection {
    int connection_id;
    char **server_to_client;
    char **client_to_server;
} connection_t;


int create_connection_pool(connection_t **connection);

int clean_connections();








#endif