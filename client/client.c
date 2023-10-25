#include <ctype.h>
#include "client.h"


void prepare_client(connection_t connection) {

    const int *client_to_server = (const int *) connection.client_to_server;
    const int *server_to_client = (const int *) connection.server_to_client;

    close(client_to_server[READ_END]);
    close(server_to_client[WRITE_END]);

    //TODO: change
    char *buffer = "Test";
    send_message_to_server(client_to_server, buffer);
}


void show_menu() {
    fprintf(OUTPUT_CHANNEL, "Choose your action: \n");
    fprintf(OUTPUT_CHANNEL, "[E]nter message \n");
    fprintf(OUTPUT_CHANNEL, "[C]hoose channel \n");
    fprintf(OUTPUT_CHANNEL, "[Q]uit program \n");
    fflush(OUTPUT_CHANNEL);
}

enum ACTION get_user_action() {
    char input;
    scanf("%c \n", &input);

    switch (tolower(input)) {
        case 'e':
            return ENTER_MESSAGE;
        case 'c':
            return CHOOSE_CHANNEL;
        case 'q':
            return QUIT;
        default:
            return INVALID;
    }
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
            fprintf(ERROR_CHANNEL, "CLIENT: Error writing to pipe.");
            return;
        }
        bytes_sent += n;
    }
}