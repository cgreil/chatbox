#include "client.h"


void prepare_client(const int *client_to_server, const int *server_to_client) {

    close(client_to_server[READ_END]);
    close(server_to_client[WRITE_END]);

    //TODO: change
    char *buffer = "Test";
    send_message_to_server(client_to_server, buffer);
}

void send_message_to_server(const int *client_to_server, char *message) {
    unsigned long terminated_str_len = strlen(message);
    fprintf(stdout, "CLIENT: sending message: %s\n", message);
    ssize_t n = 0;
    size_t len = strlen(message) + 1;
    size_t bytes_sent = 0;
    while(bytes_sent < len) {
        n = write(client_to_server[1], message + bytes_sent, len - bytes_sent);
        if (n < 0) {
            fprintf(stderr, "CLIENT: Error writing to pipe.");
            return;
        }
        bytes_sent += n;
    }

}