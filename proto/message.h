#ifndef MESSAGE_H
#define MESSAGE_H

#define OUTPUT_CHANNEL stdout
#define INPUT_CHANNEL stdin
#define ERROR_CHANNEL stderr

#include <time.h>
#include <string.h>

#include "../client/user.h"
#include "message.pb-c.h"


typedef enum {
    SERVER_MESSAGE,
    PRIVATE_MESSAGE,
    PUBLIC_MESSAGE,
    INVALID_MESSAGE
} MESSAGE_TYPE_T;

typedef struct {
    MESSAGE_TYPE_T message_type;
    user_t *sender;
    char *message_content;
    size_t content_length;
    struct tm *creation_timestamp;
} message_t;

typedef struct {
    // storing the stringified, i.e. serialized message to be sent
    char *msg_string;
    size_t msg_length;
} serialized_msg_t;

int create_message(message_t *message, MESSAGE_TYPE_T msg_type, user_t *sending_user, char **message_content,
                   size_t content_length);


int send_message(message_t *message);

int serialize_message(serialized_msg_t *serial_msg, message_t *message_to_serialize);

int deserialize_message(serialized_msg_t *serial_msg, message_t *deserialized_message);

int destroy_message(message_t *message);

static int check_message_valid(message_t *message);

int time_from_string(char *time_string, struct tm *time_struct);

int string_from_time(struct tm *time_struct, char *time_string);

#endif
