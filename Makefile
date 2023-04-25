# Copyright 2023 Sampetru Mario
CC=gcc
CFLAGS=-std=c99 -Wall -Wextra
LOAD=load_balancer
SERVER=server
HASHTABLE=hashtable
GENERALS=generals

.PHONY: build clean

build:
	$(CC) $(CFLAGS) *.c -o tema2
zip:
	zip tema2.zip *.c *.h Makefile
clean:
	rm -f *.o tema2 *.h.gch
