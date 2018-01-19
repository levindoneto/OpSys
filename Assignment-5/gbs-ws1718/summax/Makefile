# GBS Shell Makefile

CC = gcc
CFLAGS  = -Wall -g
LDFLAGS = -lpthread
OBJ = main.o summax.o

all: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o main $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c $<

clean:
	rm -rf *.o main
