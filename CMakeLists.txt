cmake_minimum_required(VERSION 3.26)
project(communication C)

set(CMAKE_C_STANDARD 11)

add_executable(communication
        main.c
        client/client.h
        connection.h
        server/server.h
        server/server.c
        client/client.c)
