CFLAGS = -Wall -Werror -O0 -g3
LDFLAGS = -lprotobuf-c

test: protobuf_test

protobuf_test: message.o message-pb.o protobuf_test.o  
	$(CC) $(CFLAGS) message.o message-pb.o protobuf_test.o $(LDFLAGS) -o $@ 

protobuf_test.o: protobuf_test.c 
	$(CC) $(CFLAGS) -c $< -o $@

message.o: message.c message.h
	$(CC) $(CFLAGS) -c $< -o $@  

message-pb.o: message.pb-c.c message.pb-c.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f message.o
	rm -f message-pb.o
	rm -f protobuf_test.o
	rm -f protobuf_test
