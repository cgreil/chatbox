#ifndef USER_H
#define USER_H

#define MAX_USERNAME_SIZE 256

typedef struct {
    unsigned int user_id;
    char *username;
} user_t;

int create_user(user_t *user, char *username);

int change_username(user_t *user, char *new_username);

int delete_user(user_t *user);





#endif