/* Copyright 2023 Sampetru Mario */
#include <stdlib.h>
#include <string.h>

#include "load_balancer.h"
#include "utils.h"
#include "server.h"

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

typedef struct {
	unsigned int label;
	server_memory *server;
} hash_ring_t;

struct load_balancer {
	hash_ring_t *hash_ring;
	unsigned int hash_ring_size;
	unsigned int hash_ring_capacity;
};

load_balancer *init_load_balancer()
{
	load_balancer *lb = NULL;

	lb = malloc(sizeof(*lb));
	DIE(!lb, "malloc failed");

	lb->hash_ring_capacity = INIT_SIZE;
	lb->hash_ring_size = 0;

	lb->hash_ring = malloc(INIT_SIZE * sizeof(hash_ring_t));
	DIE(!lb->hash_ring, "malloc failed");

	return lb;
}

void swap_data(hash_ring_t *a, hash_ring_t *b)
{
    hash_ring_t tmp = *a;
    *a = *b;
    *b = tmp;
}

void possible_realloc(load_balancer *main)
{
	if (main->hash_ring_capacity == main->hash_ring_size) {
		main->hash_ring_capacity *= INIT_SIZE;
		hash_ring_t *tmp = realloc(main->hash_ring, main->hash_ring_capacity
								   * sizeof(hash_ring_t));

		DIE(!tmp, "realloc faield");

		main->hash_ring = tmp;
	}
}

void rebalance(load_balancer *main, unsigned int pos)
{	
	ll_node_t *node = NULL;
	unsigned int j = 0;
	while (j < main->hash_ring[pos + 1].server->ht->size) {
		node = main->hash_ring[pos + 1].server->ht->buckets[j]->head;

		while (node) {
			server_store(main->hash_ring[pos].server, ((info_t *)node)->key, ((info_t *)node)->value);
			server_remove(main->hash_ring[pos + 1].server, ((info_t *)node)->key);
			node = node->next;
		}

		j++;
	}
}

void add_first_server(load_balancer *main, unsigned int label, server_memory *
					  new_server)
{
	main->hash_ring[0].label = label;
	main->hash_ring[0].server = new_server;
	main->hash_ring_size++;
}

void add_new_server(load_balancer *main, unsigned int label, server_memory *
					new_server)
{
	unsigned int pos = 0;

	while (pos < main->hash_ring_size &&
			hash_function_servers(&label) >=
			hash_function_servers(&main->hash_ring[pos].label)) {
		
		if (hash_function_servers(&label) ==
			hash_function_servers(&main->hash_ring[pos].label)) {
			if (label % TEN_TO_FIFTH > main->hash_ring->label %
				TEN_TO_FIFTH) {
					pos++;
					break;
				} else {
					break;
				}
			}
		pos++;
	}

	for (unsigned int j = main->hash_ring_size; j > pos; j--)
		main->hash_ring[j] = main->hash_ring[j - 1];

	main->hash_ring[pos].label = label;
	main->hash_ring[pos].server = new_server;
	main->hash_ring_size++;

	if (pos + 1 < main->hash_ring_size)
		if (main->hash_ring[pos + 1].server->ht->size)
			rebalance(main, pos);
}

void loader_add_server(load_balancer *main, int server_id)
{	
	// Verific daca este nevoie de realocare.
	possible_realloc(main);

    for (int i = 0; i < NR_REPLICAS; i++) {
        server_memory *new_server = init_server_memory();

        unsigned int label = i * TEN_TO_FIFTH + server_id;

		// Adaug primul server in hash ring.
        if (!main->hash_ring_size)
			add_first_server(main, label, new_server);
        else
			add_new_server(main, label, new_server);
    }
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
	for (unsigned int i = 0; i < main->hash_ring_size; i++) {
		printf("hash_label :  0x%x     ,   server_id: %d\n", hash_function_servers(&main->hash_ring[i].label), main->hash_ring[i].label % TEN_TO_FIFTH);
		free_server_memory(main->hash_ring[i].server);
	}
	
	// free(main->hash_ring->server);

	free(main->hash_ring);

	free(main);
}
