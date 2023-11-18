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
#define ERROR_CHANNEL stderr

#define MAX_USERNAME_SIZE 64

enum ACTION {
    SEND_MESSAGE = 0,
    CHOOSE_USERNAME = 1,
    QUIT = 2,
    INVALID = 3
};



void prepare_client(connection_t connection);

void send_message_to_server(const int *client_to_server, char *message);

void show_menu();

enum ACTION get_user_action();

void handle_action(enum ACTION action, const int *client_to_server);

void clear_screen();

void start_client(connection_t connection);



#endif