#ifndef SERVER_H
#define SERVER_H

#include "../util/connection.h"

void receive_message_from_client(const int *client_to_server);

void listen_to_messages();

void prepare_server(connection_t connection);








#endif