CC = gcc
CFLAGS = -Wall -lpthread -lncurses -g -Og
DEBUG = TEST_FLAG

all: build

build: game.c server.c
	$(CC) $^ $(CFLAGS) -D $(DEBUG) -o tanks

release: game.c server.c
	$(CC) $^ $(CFLAGS) -o tanks
