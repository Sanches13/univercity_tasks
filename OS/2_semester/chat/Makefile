CC = gcc

CFLAGS = -lpthread -lncurses

FILE = chat.c

OUT = server

all: build

build:
	$(CC) $(FILE) $(CFLAGS) -o $(OUT)

clean:
	rm-rf $(OUT)
