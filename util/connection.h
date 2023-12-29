#ifndef CONNECTION_H
#define CONNECTION_H

#define MAX_CONNECTIONS 9

#define STC_CONNECTION_PREFIX "/tmp/STC_"
#define CTS_CONNECTION_PREFIX "/tmp/CTS_"

typedef struct connection {
    int connection_id;
    char **server_to_client;
    char **client_to_server;
} connection_t;


connection_t **create_connection_pool();

int clear_connection_pool();








#endif