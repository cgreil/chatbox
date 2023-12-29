#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "server.h"
#include "../util/connection.h"


#define BUFFER_SIZE 512

#define READ_END 0
#define WRITE_END 1

int main() {
    fprintf(stdout, "Welcome to the server app \n");
    // open pool of connections
    connection_t **connections = create_connection_pool();
    open_all_connections(connections);




    clear_connection_pool();


}

void prepare_server(connection_t connection) {

    //receive_message_from_client(connection.client_to_server);
}

void open_all_connections(connection_t **connections) {

    fprintf(stdout, "Opening all ports ... \n");
    for (int i = 0; i < MAX_CONNECTIONS; ++i) {
        //TODO: open server to client as writeonly and client to server as readonly
        fopen(*connections[i]->server_to_client, "rw");
        fopen(*connections[i]->client_to_server, "rw");
        fprintf(stdout, "Opened port %d \n", i);
    }
    fprintf(stdout, "Opened ports ... \n");

}


void receive_message_from_client(const int *client_to_server) {
    char buffer[BUFFER_SIZE];
    char next_char;
    unsigned int char_count = 0;

    while(true) {
        ssize_t num_read = read(client_to_server[READ_END], &next_char, 1);
        if (num_read == -1) {
            fprintf(stderr, "Server: Error reading from pipe.\n");
            return;
        }
        else if (num_read == 0) {
            continue;
        }

        if(next_char == '\0') {
            break;
        }
        else if (char_count >= BUFFER_SIZE) {
            fprintf(stderr, "SERVER: Read message size larger than buffer size.");
            return;
        }
        else {
            buffer[char_count] = next_char;
            char_count++;
        }
    }

    printf("SERVER: Received message: %s \n", buffer);
}