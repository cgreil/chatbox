#ifndef USER_H
#define USER_H

#include "../util/console.h"

#define OUTPUT_CHANNEL stdout
#define INPUT_CHANNEL stdin
#define ERROR_CHANNEL stderr

#define MAX_USERNAME_SIZE 64

typedef struct {
    unsigned int user_id;
    char *username;
} user_t;

int create_user(user_t *user, char *username);

int change_username(user_t *user);

int destroy_user(user_t *user);

int copy_user(user_t *src, user_t *dest);

int get_username_input(char *username_buffer);


#endif