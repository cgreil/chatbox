#ifndef MESSAGE_H
#define MESSAGE_H

#define OUTPUT_CHANNEL stdout
#define INPUT_CHANNEL stdin
#define ERROR_CHANNEL stderr

#define MAX_MSG_SIZE 2048

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
    void *frames;
    size_t length;
} serial_packet_t;

int create_message(message_t *message, MESSAGE_TYPE_T msg_type, user_t *sending_user, char *message_content,
                   size_t content_length);


int send_message(message_t *message);

int serialize_message(serial_packet_t *packet, message_t *message);

int deserialize_message(message_t *message, serial_packet_t *packet);

int destroy_message(message_t *message);


#endif
