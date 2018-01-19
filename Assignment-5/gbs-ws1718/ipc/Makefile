# IPC Makefile

CC = gcc
CFLAGS  = -Wall -g
LDFLAGS = -lpthread -lrt
OBJ_DAEMON = ipcd.o
OBJ_CLIENT = ipc-client.o

all: ipcd ipc-client

ipcd: $(OBJ_DAEMON)
	$(CC) $(CFLAGS) $(LDFLAGS) -o ipcd $(OBJ_DAEMON)

ipc-client: $(OBJ_CLIENT)
	$(CC) $(CFLAGS) $(LDFLAGS) -o ipc-client $(OBJ_CLIENT)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm *.o ipc-client ipcd
