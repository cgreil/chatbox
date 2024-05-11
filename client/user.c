#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "user.h"


int create_user(user_t *user, char *username) {

    if (user == NULL) {
        fprintf(ERROR_CHANNEL, "user cannot be null \n");
        return -1;
    }
    char *username_buffer = malloc(MAX_USERNAME_SIZE);
    strncpy(username_buffer, username, MAX_USERNAME_SIZE);
    user->username = username_buffer;

    fprintf(OUTPUT_CHANNEL, "User %s was successfully created \n", user->username);
    return 0;
}

int change_username(user_t *user) {

    if (user == NULL) {
        fprintf(ERROR_CHANNEL, "User cannot be null \n");
        return -1;
    }

    fprintf(OUTPUT_CHANNEL, "Input new username: \n");
    char *username_buffer = malloc(MAX_USERNAME_SIZE);
    get_username_input(username_buffer);

    char old_username[MAX_USERNAME_SIZE];
    strncpy(old_username, user->username, MAX_USERNAME_SIZE);

    user->username = username_buffer;
    fprintf(OUTPUT_CHANNEL, "Successfully renamed user %s to %s\n", old_username, username_buffer);
    fflush(OUTPUT_CHANNEL);

    return 0;
}

int destroy_user(user_t *user) {

    if (user == NULL) {
        fprintf(ERROR_CHANNEL, "User cannot be null \n");
        return -1;
    }

    char *username = (char *) malloc(MAX_USERNAME_SIZE);
    strncpy(username, user->username, MAX_USERNAME_SIZE);
    memset(user, 0, sizeof(user_t));
    fprintf(OUTPUT_CHANNEL, "Successfully deleted user %s \n", username);
    free(username);

    return 0;
}

int copy_user(user_t *src, user_t *dest) {

    if (src == NULL) {
        fprintf(ERROR_CHANNEL, "Source may not be null \n");
        return -1;
    }
    if (dest == NULL) {
        fprintf(ERROR_CHANNEL, "Destination may not be null \n");
        return -1;
    }

    strncpy(dest->username, src->username, MAX_USERNAME_SIZE);
    dest->user_id = src->user_id;

    return 0;
}


int get_username_input(char *username_buffer) {
    // set input to end of stdin so that previously written newline is not read
    flush_input();
    fprintf(OUTPUT_CHANNEL, "Please enter your username: \n");
    char *msg = fgets(username_buffer, MAX_USERNAME_SIZE, stdin);
    if (msg == NULL) {
        fprintf(ERROR_CHANNEL, "Could not read username \n");
        return -1;
    }
    return 0;
}







