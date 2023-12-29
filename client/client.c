#include <ctype.h>
#include "client.h"

int main() {
    fprintf(stdout, "Welcome to the client app \n");

    run_client();

    exit(EXIT_SUCCESS);
}


void run_client() {

    connection_t connection;

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
        case 'q':
            return QUIT;
        default:
            return INVALID;
    }
}

void handle_action(ACTION_T action, connection_t connection) {

    clear_screen();

    switch (action) {
        case SEND_MESSAGE: {
            /*
            fprintf(OUTPUT_CHANNEL, "Enter your message: \n");
            char buffer[BUFFER_SIZE];
            size_t message_size = BUFFER_SIZE;
            ssize_t num_read = getline((char **) &buffer, &message_size, stdin);
            if (num_read == -1) {
                fprintf(ERROR_CHANNEL, "Error encountered reading user input \n");
                break;
            }
            send_message_to_server(connection, buffer);
             */
            fprintf(stdout, "Handling action SEND_MESSAGE \n");
            break;
        }
        case CHOOSE_USERNAME: {
            /*
            fprintf(OUTPUT_CHANNEL, "Enter your username: \n");
            char buffer[MAX_USERNAME_SIZE];
            size_t username_size = MAX_USERNAME_SIZE;
            ssize_t num_read = getline((char **) &buffer, &username_size, stdin);
            if (num_read == -1) {
                fprintf(ERROR_CHANNEL, "Error encountered reading username");
                break;
            }
            //TODO: Change uname
             */
            fprintf(stdout, "Handling action CHANGE_USERNAME \n");
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

void clear_screen() {
    printf("\x1b[H\x1b[J");
}