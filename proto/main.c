#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>

#include "message.h"

int main(void) {
   fprintf(OUTPUT_CHANNEL, "Setting up protobuf tests");

   // setup user
   user_t user;
   user.user_id = 1;
   user.username = "testuser";

   // setup timestamp
   time_t rawtime;
   time(&rawtime);
   struct tm* time = localtime(&rawtime);

    // setup message
    MESSAGE_TYPE_T msg_type = PUBLIC_MESSAGE;
    char *msg_content = "Now I have become Death";
    size_t msg_content_len = strlen(msg_content);
    
    //create message
    message_t message;
    int create_ret = create_message(&message, msg_type, &user, msg_content, msg_content_len);
    if (create_ret == -1) {
        fprintf(ERROR_CHANNEL, "Something went wrong when creating message");
        exit(EXIT_FAILURE);
    }

    // setup serialized msg
    serial_packet_t packet;
    // do serialization
    serialize_message(&packet, &message);
    // original unserialized message can be freed here
    destroy_message(&message);

    assert(&packet != NULL);
    assert(packet.frames != NULL);
    assert(packet.length > 0);

    fprintf(OUTPUT_CHANNEL, "Serialized packets: \n");
    fwrite(&packet.frames, 1, packet.length, OUTPUT_CHANNEL);
    fprintf(OUTPUT_CHANNEL, " \n End of output \n");


    fprintf(OUTPUT_CHANNEL, "Sending packet ... \n");
    // simulating sending packet by doing memcopy into new struct
    //serial_packet_t received_packet;
    //received_packet.frames = malloc(packet.length);
    //memcpy(received_packet.frames, packet.frames, packet.length);
    

    message_t *deserialized_message = malloc(sizeof(message_t));
    deserialized_message->sender = malloc(sizeof(user_t));
    deserialized_message->creation_timestamp = malloc(sizeof(struct tm));
    
    deserialize_message(deserialized_message, &packet);    
    
    fprintf(OUTPUT_CHANNEL, "Deserialized message");
    fprintf(OUTPUT_CHANNEL, "%s: %s \n", deserialized_message->sender->username, deserialized_message->message_content);
 


    exit(EXIT_SUCCESS);
}
