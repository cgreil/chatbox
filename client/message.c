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
    /**
     * Message creation function. Message type has to be allocated already
     * before function call
     */

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

    // get raw time first, then transform into localtime
    time_t rawtime;
    time(&rawtime);
    struct tm *timestamp = malloc(sizeof(struct tm));
    memcpy(timestamp,localtime(&rawtime), sizeof(struct tm));
    message->creation_timestamp = timestamp;
    return 0;
}

int destroy_message(message_t *message) {

    free(message->message_content);
    free(message->sender);

    memset(message, 0, sizeof(message_t));

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

static int check_message_valid(message_t *message) {

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

    size_t time_size = 30;
    char timestring[time_size];
    string_from_time(message_to_serialize->creation_timestamp, timestring);

    size_t actual_time_size = strlen(timestring);
    msg_size += actual_time_size;
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
        case INVALID_MESSAGE:
            fprintf(ERROR_CHANNEL, "Cannot send message of type NONE \n");
            free(serial_string);
            return -1;
    }

    int serialized_size = sprintf(serial_string, "%s:<> %s:<> [%s]:<> %s",
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


    // TODO: Use proper serialization technique so that <> in message will not break program
    char *delimiter = "<>";
    char *msg_token = strtok(serial_msg->msg_string, delimiter);

    if (msg_token == NULL){
        fprintf(ERROR_CHANNEL, "Error parsing message \n");
        free(deserialized_message);
        return -1;
    }

    // declare intermediary
    struct tm creation_time;
    MESSAGE_TYPE_T message_type;
    user_t sending_user;
    char **message_content;
    size_t content_length;

    // For now, use Enum variable and increment through while loop
    MESSAGE_PART_T msg_part = 0;
    while(msg_token != NULL) {
        size_t token_length = strlen(msg_token);
        switch (msg_part) {
            case TIMESTAMP: {
                // Create time struct and assign values
                int time_ret =  time_from_string(msg_token, &creation_time);
                if (time_ret == -1){
                    fprintf(ERROR_CHANNEL, "Could not parse creation timestamp \n");
                    free(deserialized_message);
                    return -1;
                }

                break;
            }
            case TYPE:
                if (strncmp(msg_token, "PRIV", 4) == 0) {
                    message_type = PRIVATE_MESSAGE;
                }
                else if (strncmp(msg_token, "PUBL", 4) == 0) {
                    message_type = PUBLIC_MESSAGE;
                }
                else if (strncmp(msg_token, "SERV", 4) == 0) {
                    message_type = SERVER_MESSAGE;
                } else {
                    fprintf(ERROR_CHANNEL, "Could not retrieve message type");
                    free(deserialized_message);
                    return -1;
                }
                break;
            case USERNAME: {
                // create user struct and copy values into dedicated memory
                strncpy(sending_user.username, msg_token, token_length);
                // for now, use id 0 always
                sending_user.user_id = 0;
            }
            break;
            case CONTENT: {
                message_content = &msg_token;
                content_length = token_length;
            }
            break;
            default:
                fprintf(ERROR_CHANNEL, "Encountered bad msg part when deserializing message \n");
                free(deserialized_message);
                return -1;
                break;
        }
        // get next token
        msg_part++;
        msg_token = strtok(NULL, delimiter);
    }

    // allocate memory for the deserialized message
    deserialized_message = malloc(sizeof(message_t));
    int creation_ret = create_message(deserialized_message, message_type, &sending_user, message_content, content_length);
    if (creation_ret == -1) {
        fprintf(ERROR_CHANNEL, "Could not create message");
        free(deserialized_message);
        return -1;
    }
    //successfully created message
    return 0;
}

int time_from_string(char *time_string, struct tm *time_struct) {
    /**
     * Naive string to time conversion
     */

    if (time_string == NULL || time_struct == NULL) {
        fprintf(ERROR_CHANNEL, "Could not parse time from string \n");
        return -1;
    }

    char * delimiter = ":";
    // initial tokenization for hour
    char *hour = strtok(time_string, delimiter);
    time_struct->tm_hour = atoi(hour);

    // second tokenization for minute
    char *minute = strtok(NULL, delimiter);
    time_struct->tm_min = atoi(minute);

    return 0;
}

int string_from_time(struct tm *time_struct, char *time_string) {

    /**
     * Naive time to string conversion
     */
    if (time_string == NULL) {
        fprintf(ERROR_CHANNEL, "Could not parse time... \n");
        return -1;
    }
    size_t max_timesize = 5;
    // output hour and minute
    strftime(time_string, max_timesize, "%k:%M", time_struct);
    return 0;
}
