#ifndef CLIENT_H
#define CLIENT_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include "../util/connection.h"
#include "../util/console.h"
#include "user.h"

#define BUFFER_SIZE 512

#define READ_END 0
#define WRITE_END 1

#define OUTPUT_CHANNEL stdout
#define INPUT_CHANNEL stdin
#define ERROR_CHANNEL stderr
#define MAX_USERNAME_SIZE 64

typedef enum {
    SEND_MESSAGE = 0,
    CHANGE_USERNAME = 1,
    QUIT = 2,
    OPEN_CONNECTION = 3,
    INVALID = 4,
    NONE = 5
}ACTION_T;

typedef struct {
    user_t *user;
    connection_t *server_connection;
} client_data_t;

int setup_client_data(client_data_t *client_data);

int run_client();

int startup_menu();

int register_user(client_data_t *client_data);

int loop_menu(client_data_t *client_data);

void show_menu();

void send_msg_to_server(connection_t *connection);

ACTION_T parse_action();

void handle_action(ACTION_T action, client_data_t *client_data);

int get_msg_input(char *msg_buffer);

int open_server_connection(connection_t *connection);

void clear_screen();


#endif
