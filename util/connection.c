//
// Created by christoph on 03.12.23.
//

#include "connection.h"
#include <stdbool.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>


connection_t get_connection_paths(int connection_id) {

    char *full_STC_string;
    char *full_CTS_string;

    char connection_suffix =  (char)((int)'0' + connection_id);

    size_t STC_string_size = sizeof(STC_CONNECTION_PREFIX) + sizeof(connection_suffix);
    size_t CTS_string_size = sizeof(CTS_CONNECTION_PREFIX) + sizeof(connection_suffix);

    full_STC_string = (char *) malloc(STC_string_size);
    full_CTS_string = (char *) malloc(CTS_string_size);

    // prepare full connection strings
    strncpy(full_STC_string, STC_CONNECTION_PREFIX, sizeof(STC_CONNECTION_PREFIX));
    strncat(full_STC_string, &connection_suffix, sizeof(connection_suffix));

    strncpy(full_CTS_string, CTS_CONNECTION_PREFIX, sizeof(CTS_CONNECTION_PREFIX));
    strncat(full_CTS_string, &connection_suffix, sizeof(connection_suffix));

    connection_t connection;

    connection.server_to_client = &full_STC_string;
    connection.client_to_server = &full_CTS_string;

}


void create_connection_pool() {
    // serverside function to create a pool of free connections
    fprintf(stdout, "Establishing ports ... \n");
    for (int i = 1; i <= MAX_CONNECTIONS; ++i) {
        connection_t connection = get_connection_paths(i);

        int stc_feedback = mkfifo(*connection.server_to_client, 0777);
        int cts_feedback = mkfifo(*connection.client_to_server, 0777);

        if (stc_feedback == -1 || cts_feedback == -1) {
            fprintf(stderr, "Encountered issue when opening ports. Exiting ... \n");
            return;
        }

    }

    fprintf(stdout, "Successfully established ports \n");
}



connection_t get_next_connection() {
    int connection_id = 1;
    bool is_connected = false;

    while (!is_connected) {

        //for now, only up to 9 connections
        if (connection_id > MAX_CONNECTIONS) {
            fprintf(stdout, "The maximum number of concurrent connections is reached. Exiting... \n");
            exit(EXIT_SUCCESS);
        }

        connection_t connection = get_connection_paths(connection_id);







    }



}