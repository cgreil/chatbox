#include <stdlib.h>
#include <stdio.h>

#include "message.h"


int create_message(message_t *message, MESSAGE_TYPE_T msg_type, user_t *sending_user, char **message_content,
                   size_t content_length) {

    if (message == NULL) {
        fprintf(ERROR_CHANNEL, "Message may not be null \n");
        return -1;
    }
    message->message_type = msg_type;

    if (sending_user == NULL) {
        message->sender = NULL;
    } else {
        user_t *sender = malloc(sizeof(user_t));
        // copy user with deepcopy so that msg is independent of calling functions space handling
        copy_user(sending_user, sender);
        message->sender = sender;
    }

    if (message_content == NULL || *message_content == NULL) {
        message->message_content = NULL;
        message->content_length = 0;
    } else {
        char *content = malloc(content_length);
        strncpy(*message_content, content, content_length);
        message->message_content = content;
    }

    time_t timestamp;
    time(&timestamp);
    message->creation_timestamp = timestamp;

    return 0;
}

int send_message(message_t *message) {

    if (message == NULL) {
        fprintf(ERROR_CHANNEL, "Message cannot be null \n");
        return -1;
    }
    if (check_message_valid(message) == -1) {
        fprintf(ERROR_CHANNEL, "Invalid message");
        return -1;
    }


    return 0;
}

int destroy_message(message_t *message) {

    free(message->message_content);
    free(message->sender);

    memset(message, 0, sizeof(message_t));

    return 0;
}

int check_message_valid(message_t *message) {

    if (message == NULL) {
        fprintf(ERROR_CHANNEL, "Message may not be null \n");
        return -1;
    }
    if (message->message_content == NULL || message->content_length == 0) {
        fprintf(ERROR_CHANNEL, "Message content may not be empty \n");
        return -1;
    }
    if (message->sender == NULL) {
        fprintf(ERROR_CHANNEL, "Message sender cannot be null");
        return -1;
    }
    // msg is valid
    return 0;
}

