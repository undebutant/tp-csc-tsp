BIN=example_msg_server example_msg_server_notify example_msg_client example_msg_unlink example_msg_server_pingpong example_msg_client_pingpong

CFLAGS=-Wall
LDFLAGS=-lrt

all: $(BIN)

%:%.c
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(BIN)