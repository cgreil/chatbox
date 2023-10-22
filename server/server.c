#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include "server.h"

#define BUFFER_SIZE 512

#define READ_END 0
#define WRITE_END 1

void prepare_server(const int *client_to_server, const int *server_to_client) {
    close(client_to_server[WRITE_END]);
    close(server_to_client[READ_END]);

    receive_message_from_client(client_to_server);
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