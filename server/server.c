#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <netinet/in.h>
#include "server.h"

int main() {
    fprintf(OUTPUT_CHANNEL, "Starting up the server... \n");

    // setting up socket of the server
    int socket_fd = setup_server_socket();
    // store file descriptors of clients
    int client_connections[MAX_CLIENTS];

    size_t num_connected = 0;
    //start listening
    while (1) {

        sleep(5);

        // accept client connections on the socket if more clients are still possible
        if (num_connected < MAX_CLIENTS) {
            struct sockaddr_in client_address;
            int addr_len = sizeof(client_address);
            int new_client_fd = accept(socket_fd, (struct sockaddr *) &client_address,
                                       (socklen_t *) &addr_len);

            if (new_client_fd == -1) {
                //fprintf(ERROR_CHANNEL, "No client connected\n");
            } else {
                fprintf(OUTPUT_CHANNEL, "Successfully accepted client \n");
                client_connections[num_connected] = new_client_fd;
                num_connected++;
            }
        }

        //read message for any of the connected clients
        for (size_t client_id = 0; client_id < num_connected; client_id++) {
            fprintf(OUTPUT_CHANNEL, "Polling connected clients for messages... \n");
            size_t charbuf_size = (BUFFER_SIZE + 1) * sizeof(char);
            char *message_buffer = calloc(charbuf_size, 1);
            ssize_t num_read_bytes = read(client_connections[client_id], message_buffer, charbuf_size);
            if (num_read_bytes > 0) {
                fprintf(OUTPUT_CHANNEL, "Received message from client %zu: %s \n", client_id, message_buffer);
            } else {
                fprintf(OUTPUT_CHANNEL, "No message received from client %zu \n", client_id);
            }
            free(message_buffer);
        }



        //close(client_fd);
    }

    

    return -1;
}

void run_server(connection_t connection) {
    fprintf(OUTPUT_CHANNEL, "Running server \n");

    fprintf(OUTPUT_CHANNEL, "Shutting down server \n");
    fflush(OUTPUT_CHANNEL);
}

int setup_server_socket() {

    int socket_fd = socket(AF_LOCAL, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if (socket_fd == -1) {
        fprintf(ERROR_CHANNEL, "Socket creation failed. \n");
        exit(EXIT_FAILURE);
    }

    // create socket address for inter-process communication
    struct sockaddr_un server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sun_family = AF_UNIX;
    strncpy(server_address.sun_path, SOCKET_PATH, sizeof(server_address.sun_path) - 1);

    // bind socket to address
    int bind_ret = bind(socket_fd, (struct sockaddr *) &server_address, sizeof(server_address) - 1);
    if (bind_ret == -1) {
        fprintf(ERROR_CHANNEL, "Binding of Socket to address failed \n");
        exit(EXIT_FAILURE);
    }

    //set client side to listen
    int listen_ret = listen(socket_fd, MAX_PENDING);
    if (listen_ret == -1) {
        fprintf(ERROR_CHANNEL, "Setting socket to listen failed \n");
        exit(EXIT_FAILURE);
    }

    return socket_fd;
}