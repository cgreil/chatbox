#ifndef SERVER_H
#define SERVER_H

#include "../util/connection.h"

void read_client_message(connection_t connection);

void listen_to_messages();

void run_server(connection_t connection);

void open_all_connections(connection_t **connections);

int setup_server_socket();





#endif