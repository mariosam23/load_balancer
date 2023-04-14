/* Copyright 2023 Sampetru Mario */
#include <stdlib.h>
#include <string.h>

#include "server.h"
#include "dictionary.h"
#include "utils.h"

struct server_memory {
	hashtable_t *ht;
};

server_memory *init_server_memory()
{
	server_memory *server = NULL;

	server = malloc(sizeof(*server));
	DIE(!server, "malloc failed");

	server->ht = ht_create(INIT_SIZE, hash_function_string,
						   compare_function_strings, key_val_free_function);

	DIE(!server->ht, "malloc failed");

	return server;
}

void server_store(server_memory *server, char *key, char *value)
{
	ht_put(server->ht, key, strlen(key) + 1, value, strlen(value) + 1);
}

char *server_retrieve(server_memory *server, char *key)
{
	char *value = (char *)ht_get(server->ht, key);

	return value;
}

void server_remove(server_memory *server, char *key)
{
	ht_remove_entry(server->ht, key);
}

void free_server_memory(server_memory *server)
{
	ht_free(server->ht);
	free(server);
}
