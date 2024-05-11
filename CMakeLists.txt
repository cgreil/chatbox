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
                util/console.h
                util/console.c
        client/user.h
        client/user.c
        client/message.h
        client/message.c
        util/console.c

)

add_executable(server
                server/server.h
                server/server.c
                util/connection
)

add_executable(test test/message_tests.c)
target_link_libraries(test check)
