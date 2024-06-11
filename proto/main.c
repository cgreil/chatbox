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

    assert(&packet != NULL);
    assert(packet.frames != NULL);
    assert(packet.length > 0);

    fprintf(OUTPUT_CHANNEL, "Serialized packets: \n");
    fwrite(&packet.frames, 1, packet.length, OUTPUT_CHANNEL);

    exit(EXIT_SUCCESS);
}
