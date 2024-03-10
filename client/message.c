#include <stdlib.h>
#include <stdio.h>

#include "message.h"


typedef enum {
    TIMESTAMP = 0,
    USERNAME = 1,
    TYPE = 2,
    CONTENT = 3
}MESSAGE_PART_T;

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

    serialized_msg_t serialized_message;
    int ser_ret = serialize_message(&serialized_message, message);
    if (ser_ret == -1){
        fprintf(ERROR_CHANNEL, "Sending message failed");
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

int serialize_message(serialized_msg_t *serial_msg, message_t *message_to_serialize) {

    if (message_to_serialize == NULL) {
        fprintf(ERROR_CHANNEL, "Message cannot be null! \n");
        return -1;
    }
    if (serial_msg == NULL) {
        fprintf(ERROR_CHANNEL, "Serialized Message has to be initialized! \n");
        return -1;
    }

    size_t msg_size = 0;
    msg_size += message_to_serialize->content_length;
    msg_size += MAX_USERNAME_SIZE;
    msg_size += sizeof(MESSAGE_TYPE_T);

    // TODO: put into external function
    size_t time_size = 30;
    char timestring[time_size];
    struct tm *loc_time = localtime(&message_to_serialize->creation_timestamp);
    strftime(timestring, time_size, "%Y-%m-%d: %H:%M", loc_time);

    msg_size += sizeof(time_size);

    char *serial_string = (char *) malloc(msg_size);

    // TODO: put into external function
    char *type_string;
    switch (message_to_serialize->message_type) {
        case PUBLIC_MESSAGE:
            type_string = "PUBL";
            break;
        case PRIVATE_MESSAGE:
            type_string = "PRIV";
            break;
        case SERVER_MESSAGE:
            type_string = "SERV";
            break;
        case NONE:
            fprintf(ERROR_CHANNEL, "Cannot send message of type NONE \n");
            return -1;
    }

    int serialized_size = sprintf(serial_string, "%s: %s: [%s]: %s",
                                  timestring,
                                  message_to_serialize->sender->username,
                                  type_string,
                                  message_to_serialize->message_content);

    if (serialized_size <= 0) {
        fprintf(ERROR_CHANNEL, "Serializing the message failed");
        free(serial_string);
        return -1;
    }

    serial_msg->msg_string = serial_string;
    serial_msg->msg_length = serialized_size;

    return 0;
}

int deserialize_message(serialized_msg_t *serial_msg, message_t *deserialized_message) {

    if (serial_msg == NULL) {
        fprintf(ERROR_CHANNEL, "Serialized msg cannot be null \n");
        return -1;
    }

    if (deserialized_message == NULL) {
        fprintf(ERROR_CHANNEL, "deserialized msg has to be initialized \n");
        return -1;
    }

    // allocate memory for the deserialized message
    deserialized_message = malloc(sizeof(message_t));

    // Prototyped tokenization using : as delimiter
    // TODO: Use proper serialization technique so that : in message will not break program
    char *delimiter = ":";
    char *msg_token = strtok(serial_msg->msg_string, delimiter);

    if (msg_token == NULL){
        fprintf(ERROR_CHANNEL, "Error parsing message \n");
        free(deserialized_message);
        return -1;
    }

    MESSAGE_PART_T msg_part = 0;
    while(msg_token != NULL) {
        size_t token_length = strlen(msg_token);
        switch (msg_part) {
            case TIMESTAMP:
                break;
            case USERNAME:
                break;
            case TYPE:
                break;
            default:
                fprintf(ERROR_CHANNEL, "Encountered bad msg part when deserializing message \n");
                return -1;
                break;

        }



        msg_token = strtok(NULL, delimiter);
    }





    return 0;
}
