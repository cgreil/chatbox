#include <stdlib.h>
#include <stdio.h>

#include "message.h"
#include "message.pb-c.h"

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

int serialize_message(message_t *message) {
    
    
    if (message == NULL) {
        fprintf(ERROR_CHANNEL, "Message cannot be null! \n");
        return -1;
    }
    // Start of protobuf sequence
    SerializedUser serial_user = SERIALIZED_USER__INIT;
    SerializedTimestamp serial_time = SERIALIZED_TIMESTAMP__INIT;
    SerializedMessage serial_message = SERIALIZED_MESSAGE__INIT;

    // serialize user
    serial_user.id = message->sender->user_id;  
    serial_user.username = message->sender->username;

    // serialize timestamp
    serial_time.tm_sec = message->creation_timestamp->tm_sec;
    serial_time.tm_min = message->creation_timestamp->tm_min;
    serial_time.tm_hour = message-> creation_timestamp->tm_hour;
    serial_time.tm_mday = message->creation_timestamp->tm_mday;
    serial_time.tm_mon = message->creation_timestamp->tm_mon;
    serial_time.tm_year = message->creation_timestamp->tm_year;
    serial_time.tm_wday = message->creation_timestamp->tm_wday;
    serial_time.tm_yday = message->creation_timestamp->tm_yday;
    serial_time.tm_isdst = message->creation_timestamp->tm_isdst;

    // serialize full message TODO: switch case for different types
    // Convert Message type to serialized message type
    switch (message->message_type) {
        case SERVER_MESSAGE:
            serial_message.message_type = MESSAGE_TYPE__SERVER_MESSAGE;
        break;
        case PRIVATE_MESSAGE:
            serial_message.message_type = MESSAGE_TYPE__PRIVATE_MESSAGE;
        break;
        case PUBLIC_MESSAGE: 
            serial_message.message_type = MESSAGE_TYPE__PUBLIC_MESSAGE;
        break;
        case INVALID_MESSAGE:
        default:
            serial_message.message_type = MESSAGE_TYPE__INVALID_MESSAGE;
        break;
    }
    serial_message.user_type = &serial_user;
    serial_message.message_content = message->message_content;
    serial_message.content_length = message->content_length;
    serial_message.creation_timestamp = &serial_time;
    
    size_t serial_len = serialized_message__get_packed_size(&serial_message); 

    if (serial_len <= 0) {
        fprintf(ERROR_CHANNEL, "Message serialization failed \n");
        return -1;
    }

    // allocate memory and do serialization
    void *serial_message_buffer = malloc(serial_len);
    size_t packed_length = serialized_message__pack(&serial_message, serial_message_buffer); 
    
    fprintf(ERROR_CHANNEL,"Writing %zu serialized bytes\n", packed_length); 
    fwrite (serial_message_buffer, packed_length, 1, OUTPUT_CHANNEL);  
  
    free(serial_message_buffer); 
    //TODO: Instead of directly writing to stdout, return information in a wrapper struct
    return 0;
}

int deserialize_message(void *serial_msg, message_t *deserialized_message) {

    if (serial_msg == NULL) {
        fprintf(ERROR_CHANNEL, "Serialized msg cannot be null \n");
        return -1;
    }

    if (deserialized_message == NULL) {
        fprintf(ERROR_CHANNEL, "deserialized msg has to be initialized \n");
        return -1;
    }

    


    return 0;
}


