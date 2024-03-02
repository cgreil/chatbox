#ifndef USER_H
#define USER_H

#define OUTPUT_CHANNEL stdout
#define INPUT_CHANNEL stdin
#define ERROR_CHANNEL stderr

#define MAX_USERNAME_SIZE 256

typedef struct {
    unsigned int user_id;
    char *username;
} user_t;

int create_user(user_t *user, char *username);

int change_username(user_t *user, char *new_username);

int destroy_user(user_t *user);

int copy_user(user_t *src, user_t *dest);



#endif