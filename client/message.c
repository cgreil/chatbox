#include <stdlib.h>
#include <stdio.h>

#include "message.h"


int create_message(message_t *message,
                   MESSAGE_TYPE_T msg_type,
                   user_t *sending_user,
                   char **message_content,
                   size_t content_length,
                   time_t creation_timestamp) {

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




}

int send_message(message_t *message) {

    if (message == NULL) {
        fprintf(OUTPUT_CHANNEL, "Message cannot be null \n");
        return -1;
    }

    int msg_validity = check_valid_message(message);
    if (msg_validity == -1) {
        fprintf(ERROR_CHANNEL, "Invalid message");
        return -1;
    }



    return 0;
}




int check_valid_message(message_t *message) {

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
