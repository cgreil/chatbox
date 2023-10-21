#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

#include <sys/wait.h>
#include <sys/types.h>

#define NUM_PROCESSES 2

#define READ_END 0
#define WRITE_END 1
#define BUFFER_SIZE 512




void create_processes(size_t num_processes, pid_t child_pids[NUM_PROCESSES]);

bool is_main_process(const pid_t child_pids[NUM_PROCESSES]);

void receive_message(const int *client_to_server, const int *server_to_client);

void wait_children_and_exit();

int main() {

    // 2 pipes for bidirectional communication
    int client_to_server[2];
    if (pipe(client_to_server) == -1) {
        fprintf(stderr, "Client to Server communication could not be establishes. Exiting. \n");
        exit(EXIT_FAILURE);
    }
    int server_to_client[2];
    if (pipe(server_to_client) == -1) {
        fprintf(stderr, "Server to Client communication could not be establishes. Exiting. \n");
        exit(EXIT_FAILURE);
    }

    pid_t process_ids[NUM_PROCESSES];
    create_processes(NUM_PROCESSES, process_ids);

    bool is_main = is_main_process(process_ids);
    if(is_main) {
        receive_message(client_to_server, server_to_client);
        wait_children_and_exit();
    }
    else {


    }


    return 0;

}

void wait_children_and_exit() {
    int wait_status = 0;
    printf("Waiting for children \n");
    while ((wait(&wait_status)) > 0);
    printf("finished waiting");
    // server cleanup
    exit(EXIT_SUCCESS);
}

void receive_message(const int *client_to_server, const int *server_to_client) {
    char buffer[BUFFER_SIZE];
    close(client_to_server[WRITE_END]);
    close(server_to_client[READ_END]);
    char next_char;
    unsigned int char_count = 0;
    while(true) {
        ssize_t num_read = read(client_to_server[READ_END], &next_char, 1);
        if (num_read == -1) {
            fprintf(stderr, "Error reading from pipe. \n");
            return;
        }
        else if (num_read == 0) {
            continue;
        }

        if(next_char == '\0') {
            break;
        }
        else if (char_count >= BUFFER_SIZE) {
            fprintf(stderr, "Server: Read message size larger than buffer size.");
            return;
        }
        else {
            buffer[char_count] = next_char;
            char_count++;
        }
    }
    fprintf(stdout, "Server received message: %s", buffer);
}

void create_processes(size_t num_processes, pid_t child_pids[NUM_PROCESSES]) {

    if(num_processes <= 0) {
        return;
    }

    fprintf(stdout, "Creating processes \n");
    for(size_t p_num = 0; p_num < num_processes; ++p_num) {
        pid_t process_id = fork();
        if (process_id < 0) {
            fprintf(stderr, "Process creation failed. Exiting");
            exit(EXIT_FAILURE);
        }
        else if (process_id == 0) {
            break;
        }
        else {
            child_pids[p_num] = process_id;
            fprintf(stdout, "Created process with pid %d \n", process_id);
            continue;
        }
    }
}

bool is_main_process(const pid_t child_pids[NUM_PROCESSES]) {
    for (size_t i = 0; i < NUM_PROCESSES; ++i) {
        if (child_pids[i] == 0) {
            return false;
        }
    }
    return true;
}




