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

    // TODO: Cleanup
    //on_exit(cleanup_server, client_connections, num_connected, socket_fd);
    //start listening
    while (1) {

        sleep(5);
        // accept client connections on the socket if more clients are still possible

        if (num_connected < MAX_CLIENTS) {
            int new_client_fd = handle_client_connections(socket_fd);
            client_connections[num_connected] = new_client_fd;
            num_connected++;
        } else {
            fprintf(OUTPUT_CHANNEL, "Maximal number of clients connected: %zu/%d \n", num_connected, MAX_CLIENTS);
        }

        fd_set readset;
        int num_readable_fds = get_available_sockets(&readset, client_connections, num_connected);
        if (num_readable_fds == 0) {
            // no messages waiting to be received
            continue;
        } else {
            fprintf(OUTPUT_CHANNEL, "Receiving client messages \n");
        }
        for (int client_id = 0; client_id < num_connected; ++client_id) {

            // check whether fd is in readset
            if (!FD_ISSET(client_connections[client_id], &readset)) {
                continue;
            }
            int message_handling = read_msg_from_client(client_connections[client_id], client_id + 1);

        }

    }

    return 0;
}

void run_server(connection_t connection) {
    fprintf(OUTPUT_CHANNEL, "Starting up server \n");


    fprintf(OUTPUT_CHANNEL, "Shutting down server \n");
    fflush(OUTPUT_CHANNEL);
}

void cleanup_server(int *client_fd, int num_connected, int socket_fd) {
    //TODO: Cleanup and add with on_exit
    //close all file descriptors to clients
    for (int i = 0; i < num_connected; ++i) {
        close(client_fd[i]);
    }
    close(socket_fd);
    unlink(SOCKET_PATH);
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

int get_available_sockets(fd_set *sockets, int *monitored_descriptors, size_t num_monitored) {
    //Replace accept by select to monitor multiple file descriptors:
    //fd_set sockets;
    // set read timeout for after which reading on a single socket stops

    //initially reset socket set
    FD_ZERO(sockets);

    //register all connected fds into the readset
    for (size_t i = 0; i < num_monitored; ++i) {
        FD_SET(*(monitored_descriptors + i), sockets);
    }

    struct timeval timeout;
    timeout.tv_sec = SELECT_TIMEOUT_SEC;
    timeout.tv_usec = SELECT_TIMEOUT_MICROSEC;

    // get all fds that contain data to be read
    int num_read_fds = select(FD_SETSIZE, sockets, (fd_set *) NULL,
                              (fd_set *) NULL, &timeout);
    return num_read_fds;
}

int handle_client_connections(int socket_fd) {
    struct sockaddr_in client_address;
    int addr_len = sizeof(client_address);
    int new_client_fd = accept(socket_fd, (struct sockaddr *) &client_address,
                               (socklen_t *) &addr_len);
    if (new_client_fd == -1) {
        // case where no new client is connecting
        fprintf(ERROR_CHANNEL, "No client connected\n");
    } else {
        fprintf(OUTPUT_CHANNEL, "Successfully accepted new client \n");
    }

    return 0;
}

int read_msg_from_client(int fd_to_read, int client_id) {
    size_t charbuf_size = (BUFFER_SIZE + 1) * sizeof(char);
    char *message_buffer = calloc(charbuf_size, 1);
    ssize_t num_read_bytes = read(fd_to_read, message_buffer, charbuf_size);
    if (num_read_bytes > 0 && message_buffer != NULL) {
        fprintf(OUTPUT_CHANNEL, "Received message from client %d: %s \n", client_id, message_buffer);
    } else {
        fprintf(OUTPUT_CHANNEL, "No message received from client %d \n", client_id);
    }
    free(message_buffer);

    return 0;
}
