/* Copyright 2023 Sampetru Mario */
#include <stdlib.h>
#include <string.h>

#include "load_balancer.h"
#include "dictionary.h"
#include "utils.h"

typedef struct {
	uint server_id;
	// eticheta = replica_id * 10^5  + server_id;
	uint label;
	uint replica_id;
	uint hash;
	server_memory *server;
} hash_ring_t;

struct load_balancer {
	hash_ring_t *hash_ring;
	uint hash_ring_size;
	uint hash_ring_capacity;
};

unsigned int hash_function_servers(void *a)
{
	unsigned int uint_a = *((unsigned int *)a);

	uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
	uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
	uint_a = (uint_a >> 16u) ^ uint_a;
	return uint_a;
}

unsigned int hash_function_key(void *a)
{
	unsigned char *puchar_a = (unsigned char *)a;
	unsigned int hash = 5381;
	int c;

	while ((c = *puchar_a++))
		hash = ((hash << 5u) + hash) + c;

	return hash;
}

load_balancer *init_load_balancer()
{
	load_balancer *lb = NULL;

	lb = malloc(sizeof(*lb));
	DIE(!lb, "malloc failed");



	return lb;
}

void loader_add_server(load_balancer *main, int server_id)
{
	
}

void loader_remove_server(load_balancer *main, int server_id)
{
	/* TODO 3 */
}

void loader_store(load_balancer *main, char *key, char *value, int *server_id)
{
	/* TODO 4 */
}

char *loader_retrieve(load_balancer *main, char *key, int *server_id)
{
	/* TODO 5 */
	return NULL;
}

void free_load_balancer(load_balancer *main)
{
	/* TODO 6 */
}
