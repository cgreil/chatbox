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

     serialized_message;
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

int serialize_message(serial_packet_t *packet, message_t *message) {
    
    
    if (message == NULL) {
        fprintf(ERROR_CHANNEL, "Message cannot be null! \n");
        return -1;
    }
    // Start of protobuf sequence
    SerializedUser serial_user = SERIALIZED_USER__INIT;
    SerializedTimestamp serial_time = SERIALIZED_TIMESTAMP__INIT;
    SerializedMessage serial_message = SERIALIZED_MESSAGE__INIT;

    // TODO: Improve dependency structure 
    pack_timestamp(message->creation_timestamp, &serial_time);
    pack_user(message->sender, &serial_user);
    // packing message contents for serialization
    pack_message(message, &serial_message);
    serial_message.user_type = &serial_user;
    serial_message.creation_timestamp = &serial_time;
   
    size_t serial_len = serialized_message__get_packed_size(&serial_message); 

    if (serial_len <= 0) {
        fprintf(ERROR_CHANNEL, "Message serialization failed \n");
        return -1;
    }

    // allocate memory and do serialization
    void *serial_message_buffer = malloc(serial_len);
    size_t packed_length = serialized_message__pack(&serial_message, serial_message_buffer); 
    
    // TODO: remove debug printing
    fprintf(ERROR_CHANNEL,"Writing %zu serialized bytes\n", packed_length); 
    fwrite (serial_message_buffer, packed_length, 1, OUTPUT_CHANNEL);  

    //
    packet->frames = serial_message_buffer; 
    packet->length = packed_length;
  
    free(serial_message_buffer); 
    //TODO: Instead of directly writing to stdout, return information in a wrapper struct
    return 0;
}

int pack_timestamp(struct tm *time, SerializedTimestamp *serial_time) {

    serial_time->tm_sec = time->tm_sec;
    serial_time->tm_min = time->tm_min;
    serial_time->tm_hour = time->tm_hour;
    serial_time->tm_mday = time->tm_mday;
    serial_time->tm_mon = time->tm_mon;
    serial_time->tm_year = time->tm_year;
    serial_time->tm_wday = time->tm_wday;
    serial_time->tm_yday = time->tm_yday;
    serial_time->tm_isdst = time->tm_isdst;

    return 0;
}

int pack_user(user_t *user, SerializedUser *serial_user) {

   serial_user->id = user->user_id; 
   strcpy(serial_user->username, user->username);

   return 0;
}

int pack_message(message_t *message, SerializedMessage *serial_message) {
   // Convert Message type to serialized message type
    switch (message->message_type) {
        case SERVER_MESSAGE:
            serial_message->message_type = MESSAGE_TYPE__SERVER_MESSAGE;
        break;
        case PRIVATE_MESSAGE:
            serial_message->message_type = MESSAGE_TYPE__PRIVATE_MESSAGE;
        break;
        case PUBLIC_MESSAGE: 
            serial_message->message_type = MESSAGE_TYPE__PUBLIC_MESSAGE;
        break;
        case INVALID_MESSAGE:
        default:
            serial_message->message_type = MESSAGE_TYPE__INVALID_MESSAGE;
        break;
    }
    strcpy(serial_message->message_content, message->message_content);
    serial_message->content_length = message->content_length;
 
}

int deserialize_message(message_t *message, serial_packet_t *packet){

    if (packet == NULL) {
        fprintf(ERROR_CHANNEL, "Serialized packet cannot be null \n");
        return -1;
    }

    if (message == NULL) {
        fprintf(ERROR_CHANNEL, "Deserialized msg has to be initialized \n");
        return -1;
    }

    


    return 0;
}


