# Copyright 2023 Sampetru Mario
CC=gcc
CFLAGS=-std=c99 -Wall -Wextra
LOAD=load_balancer
SERVER=server
DICTIONARY=dictionary

.PHONY: build clean

build: tema2

tema2: main.o $(LOAD).o $(SERVER).o $(DICTIONARY).o
	$(CC) $^ -o $@

main.o: main.c
	$(CC) $(CFLAGS) $^ -c

$(SERVER).o: $(SERVER).c $(SERVER).h
	$(CC) $(CFLAGS) $^ -c

$(LOAD).o: $(LOAD).c $(LOAD).h
	$(CC) $(CFLAGS) $^ -c

$(DICTIONARY).o: $(DICTIONARY).c $(DICTIONARY).h
	$(CC) $(CFLAGS) $^ -c

clean:
	rm -f *.o tema2 *.h.gch
