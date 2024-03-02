#ifndef MESSAGE_H
#define MESSAGE_H

#define OUTPUT_CHANNEL stdout
#define INPUT_CHANNEL stdin
#define ERROR_CHANNEL stderr

#include <time.h>
#include <string.h>

#include "user.h"

typedef enum {
    SERVER_MESSAGE,
    PRIVATE_MESSAGE,
    PUBLIC_MESSAGE,
    NONE
} MESSAGE_TYPE_T;

typedef struct {
    MESSAGE_TYPE_T message_type;
    user_t *sender;
    char *message_content;
    size_t content_length;
    time_t creation_timestamp;
}message_t;

typedef struct {
    // storing the stringified, i.e. serialized message to be sent
    char *msg_string;
    size_t msg_length;
}serialized_msg_t;

int create_message(message_t *message,
                   MESSAGE_TYPE_T msg_type,
                   user_t *sending_user,
                   char **message_content,
                   size_t content_length,
                   time_t creation_timestamp);
}

int send_message(message_t *message);

int serialize_message(serialized_msg_t *serial_msg, message_t *message_to_serialize);

int deserialize_message(serialized_msg_t *serial_msg, message_t *deserialized_message);

int free_message(message_t message);

static int check_valid_message(message_t *message);



#endif