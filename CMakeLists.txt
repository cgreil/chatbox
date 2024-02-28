cmake_minimum_required(VERSION 3.26)
project(communication C)

set(CMAKE_C_STANDARD 11)

if(FALSE) # fake a block comment
add_executable(dev
        main.c
        client/client.h
        util/connection.h
        server/server.h
        server/server.c
        client/client.c)
endif()


add_executable(client
                client/client.h
                client/client.c
                util/connection.h
)

add_executable(server
                server/server.h
                server/server.c
                util/connection.h
)
