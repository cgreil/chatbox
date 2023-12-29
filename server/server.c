#include <stdio.h>
#include "server.h"
#include "../util/connection.h"


#define BUFFER_SIZE 512

#define OUTPUT_CHANNEL stdout
#define INPUT_CHANNEL stdin
#define ERROR_CHANNEL stderr


int main() {
    fprintf(OUTPUT_CHANNEL, "Starting up the server... \n");
    connection_t **connections;
    //int pool_ret = create_connection_pool(connections);

    connection_t connection;
    run_server(connection);
    //open_all_connections(connections);
    //clean_connections();

    return -1;
}

void run_server(connection_t connection) {
    fprintf(OUTPUT_CHANNEL, "Running server \n");

    fprintf(OUTPUT_CHANNEL, "Shutting down server \n");
    fflush(OUTPUT_CHANNEL);
}

/*
void read_client_message(connection_t connection) {
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
 */