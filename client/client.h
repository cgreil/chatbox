#ifndef CLIENT_H
#define CLIENT_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include "../util/connection.h"

#define BUFFER_SIZE 512

#define READ_END 0
#define WRITE_END 1

#define OUTPUT_CHANNEL stdout
#define INPUT_CHANNEL stdin
#define ERROR_CHANNEL stderr

#define MAX_USERNAME_SIZE 64

typedef enum {
    SEND_MESSAGE = 0,
    CHOOSE_USERNAME = 1,
    QUIT = 2,
    OPEN_CONNECTION = 3,
    INVALID = 4,
    NONE = 5
}ACTION_T;

void run_client();

void send_message_to_server(connection_t connection, char *messages);

void show_menu();

ACTION_T get_user_action();

void handle_action(ACTION_T action, connection_t connection);

void clear_screen();




#endif