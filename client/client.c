#include <ctype.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "client.h"

#define MAX_MSG_SIZE 1024

int main() {

    fprintf(stdout, "Welcome to the client app \n");
    connection_t connection;
    run_client(&connection);

    exit(EXIT_SUCCESS);
}

void run_client(connection_t *connection) {

    ACTION_T next_action = NONE;
    while(next_action != QUIT) {
        show_menu();
        next_action = get_user_action();
        handle_action(next_action,  connection);
    }

}

void show_menu() {
    fprintf(OUTPUT_CHANNEL, "Choose your action: \n");
    fprintf(OUTPUT_CHANNEL, "[S]end message \n");
    fprintf(OUTPUT_CHANNEL, "[C]hoose username \n");
    fprintf(OUTPUT_CHANNEL, "[O]pen connection to server \n");
    fprintf(OUTPUT_CHANNEL, "[Q]uit program \n");
    fflush(OUTPUT_CHANNEL);
}

ACTION_T get_user_action() {

    char input = ' ';
    while (input == ' ' || input == '\n') {
        input = (char) fgetc(INPUT_CHANNEL);
    }

    switch (tolower(input)) {
        case 's':
            return SEND_MESSAGE;
        case 'c':
            return CHOOSE_USERNAME;
        case 'o':
            return OPEN_CONNECTION;
        case 'q':
            return QUIT;
        default:
            return INVALID;
    }
}

void handle_action(ACTION_T action, connection_t *connection) {

    if (connection == NULL) {
        fprintf(ERROR_CHANNEL, "Invalid connection. Exiting ... ");
        _exit(EXIT_FAILURE);
    }

    clear_screen();

    switch (action) {
        case SEND_MESSAGE: {

            fprintf(OUTPUT_CHANNEL, "Handling action SEND_MESSAGE \n");

            char msg_buffer[MAX_MSG_SIZE];
            // set input to end of stdin so that previously written newline is not read
            flush_input();
            fprintf(OUTPUT_CHANNEL, "Please enter your message: \n");
            char *msg = fgets(msg_buffer, MAX_MSG_SIZE, stdin);
            if (msg == NULL) {
                fprintf(ERROR_CHANNEL, "Could not read input \n");
                break;
            }

            //fprintf(OUTPUT_CHANNEL, "Message: %s", msg_buffer);
            ssize_t num_written = write(connection->connection_fd, msg_buffer, MAX_MSG_SIZE);
            if (num_written == -1) {
                fprintf(ERROR_CHANNEL, "Sending the Message failed. \n");
                break;
            }
            break;
        }
        case CHOOSE_USERNAME: {
            fprintf(OUTPUT_CHANNEL, "Handling action CHANGE_USERNAME \n");
            break;
        }

        case OPEN_CONNECTION: {

            //create socket file descriptor
            int socket_fd = socket(AF_LOCAL, SOCK_STREAM | SOCK_NONBLOCK, 0);
            if (socket_fd == -1) {
                fprintf(ERROR_CHANNEL, "Socket creation failed \n");
                break;
            }

            //create address of socket
            struct sockaddr_un client_address;
            memset(&client_address, 0, sizeof(client_address));
            client_address.sun_family = AF_UNIX;
            strncpy(client_address.sun_path, SOCKET_PATH, sizeof (client_address.sun_path) - 1);

            int connection_ret = connect(socket_fd, (struct sockaddr *) &client_address, sizeof(client_address) -1);
            if (connection_ret == -1) {
                fprintf(ERROR_CHANNEL, "Connection to server failed ... \n");
                break;
            }

            connection->connection_fd = socket_fd;
            fprintf(OUTPUT_CHANNEL, "Successfully connected to server \n");
            break;
        }
        case QUIT:
            fprintf(OUTPUT_CHANNEL, "The service will now shut down \n");
            fflush(OUTPUT_CHANNEL);
            break;
        case INVALID:
            fprintf(OUTPUT_CHANNEL, "Invalid action. Please specify a different action \n");
            break;
        case NONE:
            break;
    }
}

void send_message_to_server(connection_t connection, char *message) {
    unsigned long terminated_str_len = strlen(message);
    fprintf(stdout, "CLIENT: sending message: %s\n", message);
    ssize_t n = 0;
    size_t len = strlen(message) + 1;
    size_t bytes_sent = 0;
    /*while(bytes_sent < len) {
        n = write(client_to_server[1], message + bytes_sent, len - bytes_sent);
        if (n < 0) {
            fprintf(ERROR_CHANNEL, "CLIENT: Error writing to pipe.");
            return;
        }
        bytes_sent += n;
    }*/

}

void flush_input() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF ) { }
}

void clear_screen() {
    // Unholy magic
    printf("\x1b[H\x1b[J");
}