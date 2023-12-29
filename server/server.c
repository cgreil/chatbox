//needed for accept4()
#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <fcntl.h>
#include "server.h"
#include "../util/connection.h"

#define SOCKET_PATH "./socket"
#define MAX_PENDING 1



#define BUFFER_SIZE 512
#define NUM_MESSAGES 3

#define OUTPUT_CHANNEL stdout
#define INPUT_CHANNEL stdin
#define ERROR_CHANNEL stderr


int main() {
    fprintf(OUTPUT_CHANNEL, "Starting up the server... \n");


    int socket_fd = socket(AF_LOCAL, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if (socket_fd == -1) {
        fprintf(ERROR_CHANNEL, "Socket creation failed. \n");
        return EXIT_FAILURE;
    }

    // create socket address for inter-process communication
    struct sockaddr_un server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sun_family = AF_UNIX;
    strncpy(server_address.sun_path, SOCKET_PATH, sizeof (server_address.sun_path) - 1);

    // bind socket to address
    int bind_ret = bind(socket_fd, (struct sockaddr *) &server_address, sizeof(server_address) - 1);
    if (bind_ret == -1) {
        fprintf(ERROR_CHANNEL, "Binding of Socket to address failed \n");
        return EXIT_FAILURE;
    }

    //set client side to listen
    int listen_ret = listen(socket_fd, MAX_PENDING);
    if (listen_ret == -1) {
        fprintf(ERROR_CHANNEL, "Setting socket to listen failed \n");
        return EXIT_FAILURE;
    }

    size_t num_connected = 0;
    //start listening
    while(1) {

        sleep(1);
        fprintf(OUTPUT_CHANNEL, "Waiting for client message... \n");

        struct sockaddr_in client_address;
        int addr_len = sizeof(client_address);

        //int client_fd = accept4(socket_fd, (struct sockaddr *)&client_address,
        //(socklen_t *)&addr_len, O_NONBLOCK);

        int client_fd = accept(socket_fd, (struct sockaddr *)&client_address,
                                (socklen_t *)&addr_len);

        if (client_fd == -1) {
            fprintf(OUTPUT_CHANNEL, "Accepting client failed \n");
        }

        //read message
        char message_buffer[BUFFER_SIZE + 1];
        memset(message_buffer, '\0', sizeof(message_buffer));

        ssize_t count = read(client_fd, message_buffer, sizeof(message_buffer));
        if (count > 0) {
            fprintf(OUTPUT_CHANNEL, "Received message: %s \n", message_buffer);
            //write(client_fd, message_buffer, sizeof(message_buffer) /* echo as confirmation */
        }
        else {
            fprintf(OUTPUT_CHANNEL, "No message received \n");
        }

        close(client_fd);
    }



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