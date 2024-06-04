
CFLAGS = -Wall -Werror
all = client.o server.o user.o message.o connection.o console.o client server 

all: client server


.PHONY: clientapp
clientapp: client.o user.o message.o console.o 
	$(CC) $(CFLAGS) client.o user.o message.o console.o -o clientapp 

client.o: client/client.c client/client.h user.o message.o console.o
	$(CC) $(CFLAGS) -c $< -o $@

user.o: client/user.c client/user.h
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: serverapp
serverapp: server.o message.o console.o
	$(CC) $(CFLAGS) server.o -o serverapp 

server.o: server/server.c server/server.h 
	$(CC) $(CFLAGS) -c $< -o $@

#connection.o: util/connection.h 
#	$(CC) $(CFLAGS) -c $< -o $@

message.o: client/message.c client/message.h
	$(CC) $(CFLAGS) -c $< -o $@ 

console.o: util/console.c util/console.h
	$(CC) $(CFLAGS) -c $< -o $@
	
clean:
	rm -f client.o
	rm -f user.o
	rm -f server.o
	rm -f connection.o
	rm -f message.o
	rm -f console.o

