#include <ctype.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "client.h"

#define MAX_MSG_SIZE 1024

int main() {
    
    run_client();
    exit(EXIT_SUCCESS);
}

int run_client() {
    fprintf(OUTPUT_CHANNEL, "Starting up client \n");

    startup_menu();
    
    // setup clientside data
    client_data_t *client_data = malloc(sizeof(client_data_t));
    int setup_ret = setup_client_data(client_data);
    if (setup_ret == -1) {
        fprintf(ERROR_CHANNEL, "Client could not be established. Exiting ... \n");
        // TODO: cleanup client_data
        return EXIT_FAILURE;
    }

    register_user(client_data);

   //client main loop
    loop_menu(client_data);

    return 0;
}

int startup_menu() {
    fprintf(OUTPUT_CHANNEL, "Welcome!\n");
    fprintf(OUTPUT_CHANNEL, "Press [R] to register or press [Q] to quit the application \n");

    int user_reply = getchar();
    char action = tolower((char) user_reply);

    if (action == 'r') {
        return 1;
    }
    else if (action == 'q') {
        fprintf(OUTPUT_CHANNEL, "Shutting down ... \n");
        exit(EXIT_SUCCESS);
    }
    else {
        fprintf(OUTPUT_CHANNEL, "Bad Usage. Shutting down ... \n");
        exit(EXIT_FAILURE);
    }
}

int register_user(client_data_t *client_data) {
    // TODO: put into user.c
    // get and store username
    char username_buffer[MAX_USERNAME_SIZE];
    //flush_input(); - only if there are unwanted characters in buffer
    int username_ret = get_username_input(username_buffer);
    if (username_ret == -1) {
        fprintf(ERROR_CHANNEL, "Username input failed. Exiting ... \n");
        // TODO: cleanup
        return EXIT_FAILURE;
    }
    create_user(client_data->user, username_buffer);
    fprintf(OUTPUT_CHANNEL, "Successfully registered as user %s", client_data->user->username);
    
    return 0;
} 

int setup_client_data(client_data_t *client_data) {

    if (client_data == NULL) {
        fprintf(ERROR_CHANNEL, "Invalid Initalization of client data \n");
        return -1;
    }
    connection_t *server_connection = malloc(sizeof(connection_t));
    user_t *client_user = malloc(sizeof(user_t));

    client_data->server_connection = server_connection;
    client_data->user = client_user;

    return 0;
} 

int loop_menu(client_data_t *client_data) {
    
    fprintf(OUTPUT_CHANNEL, "%s, what do you want to do? \n", client_data->user->username);

    ACTION_T next_action = NONE;
    while(next_action != QUIT) {
        show_menu();
        next_action = parse_action();
        handle_action(next_action, client_data);
    }
    return 0;
}

void show_menu() {
    fprintf(OUTPUT_CHANNEL, "Choose your action: \n");
    fprintf(OUTPUT_CHANNEL, "[S]end message \n");
    fprintf(OUTPUT_CHANNEL, "[C]hange username \n");
    fprintf(OUTPUT_CHANNEL, "[O]pen connection to server \n");
    fprintf(OUTPUT_CHANNEL, "[Q]uit program \n");
    fflush(OUTPUT_CHANNEL);
}

void handle_action(ACTION_T action, client_data_t *client_data) {

    if (client_data == NULL) {
        fprintf(ERROR_CHANNEL, "Invalid connection. Exiting ... \n");
        exit(EXIT_FAILURE);
    }

    //clear_screen();
    switch (action) {
        case SEND_MESSAGE:
            send_msg_to_server(client_data->server_connection);
            break;
        case CHANGE_USERNAME:
            change_username(client_data->user);
            break;
        case OPEN_CONNECTION:
            open_server_connection(client_data->server_connection);
            break;
        case QUIT:
            fprintf(OUTPUT_CHANNEL, "The service will now shut down \n");
            fflush(OUTPUT_CHANNEL);
            break;
        case INVALID:
            fprintf(OUTPUT_CHANNEL, "Invalid action. Please specify a different action \n");
            fflush(OUTPUT_CHANNEL);
            break;
        case NONE:
            break;
    }
}

void send_msg_to_server(connection_t *connection) {
    fprintf(OUTPUT_CHANNEL, "Input message to send: \n");
    char *msg_buffer = calloc(MAX_MSG_SIZE, sizeof(char));
    if (get_msg_input(msg_buffer) == -1) {
        fprintf(ERROR_CHANNEL, "Could not retrieve user message \n");
        return;
    }
    //fprintf(OUTPUT_CHANNEL, "Message: %s", msg_buffer);
    ssize_t num_written = write(connection->connection_fd, msg_buffer, MAX_MSG_SIZE);
    if (num_written == -1) {
        fprintf(ERROR_CHANNEL, "Sending the Message failed. \n");
        free(msg_buffer);
        return;
    }
    free(msg_buffer);
}

int get_msg_input(char *msg_buffer) {
    // set input to end of stdin so that previously written newline is not read
    fprintf(OUTPUT_CHANNEL, "Please enter your message: \n");
    char *msg = fgets(msg_buffer, MAX_MSG_SIZE, stdin);
    flush_input();
    if (msg == NULL) {
        fprintf(ERROR_CHANNEL, "Could not read input \n");
        return -1;
    }
    return 0;
}

int open_server_connection(connection_t *connection) {
    //create socket file descriptor
    int socket_fd = socket(AF_LOCAL, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if (socket_fd == -1) {
        fprintf(ERROR_CHANNEL, "Socket creation failed \n");
        return -1;
    }
    //create address of socket
    struct sockaddr_un client_address;
    memset(&client_address, 0, sizeof(client_address));
    client_address.sun_family = AF_UNIX;
    strncpy(client_address.sun_path, SOCKET_PATH, sizeof (client_address.sun_path) - 1);

    int connection_ret = connect(socket_fd, (struct sockaddr *) &client_address, sizeof(client_address) - 1);
    if (connection_ret == -1) {
        fprintf(ERROR_CHANNEL, "Connection to server failed ... \n");
        return -1;
    }
    connection->connection_fd = socket_fd;
    fprintf(OUTPUT_CHANNEL, "Successfully queued for server connection \n");

    return 0;
}

ACTION_T parse_action() {

    char input = ' ';
    while (input == ' ' || input == '\n') {
        input = (char) fgetc(INPUT_CHANNEL);
    }
    flush_input();
    switch (tolower(input)) {
        case 's':
            return SEND_MESSAGE;
        case 'c':
            return CHANGE_USERNAME;
        case 'o':
            return OPEN_CONNECTION;
        case 'q':
            return QUIT;
        default:
            return INVALID;
    }
}

void clear_screen() {
    // Unholy magic
    printf("\x1b[H\x1b[J");
}
