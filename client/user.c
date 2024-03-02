#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "user.h"

#define OUTPUT_CHANNEL stdout
#define INPUT_CHANNEL stdin
#define ERROR_CHANNEL stderr


int create_user(user_t *user, char *username) {

    if (user == NULL) {
        fprintf(ERROR_CHANNEL, "user cannot be null \n");
        return -1;
    }
    strncpy(user->username, username, MAX_USERNAME_SIZE);
    fprintf(OUTPUT_CHANNEL, "User %s was successfully created \n", user->username);
    return 0;
}

int change_username(user_t *user, char *new_username) {

    if (user == NULL) {
        fprintf(ERROR_CHANNEL, "User cannot be null \n");
        return -1;
    }
    char *old_username = (char *) malloc(MAX_USERNAME_SIZE);
    strncpy(old_username, user->username, MAX_USERNAME_SIZE);
    strncpy(user->username, new_username, MAX_USERNAME_SIZE);
    fprintf(OUTPUT_CHANNEL, "Successfully renamed user %s to %s\n", old_username, new_username);
    free(old_username);
    return 0;
}

int delete_user(user_t *user) {

    if(user == NULL) {
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








