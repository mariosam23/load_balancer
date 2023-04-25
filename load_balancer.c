/* Copyright 2023 Sampetru Mario */

#include "load_balancer.h"
#include "utils.h"
#include "server.h"
#include "generals.h"
#include "remove.h"
#include "add.h"

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
	for (int i = 0; i < NR_REPLICAS; i++) {
		int pos = 0;
		unsigned int label = i * TEN_TO_FIFTH + server_id;
		pos = find_pos_to_remove(main, hash_function_servers(&label));

		int dest_pos;
		for (dest_pos = pos + 1; dest_pos < (int)main->hash_ring_size;
			 dest_pos++)
			if (get_server_id(main, dest_pos) != get_server_id(main, pos))
				break;

		dest_pos %= main->hash_ring_size;

		if (main->hash_ring[pos].server->ht->size)
			rebalance_removing(main, pos, dest_pos);

		for (int j = pos; j < (int)main->hash_ring_size - 1; j++)
			swap_data(&main->hash_ring[j], &main->hash_ring[j + 1]);
	}

	int size = main->hash_ring_size;
	for (int i = size - 1; i > size - 1 - NR_REPLICAS; i--) {
		free_server_memory(main->hash_ring[i].server);
		main->hash_ring_size--;
	}
}

void loader_store(load_balancer *main, char *key, char *value, int *server_id)
{
	unsigned int hash_key = hash_function_key(key);
	int pos;
	pos = find_pos(main, hash_key);

	// In caz ca pos ajunge sa depaseasca dimensiunea
	pos %= main->hash_ring_size;

	*server_id = main->hash_ring[pos].label % TEN_TO_FIFTH;

	server_store(main->hash_ring[pos].server, key, value);
}

char *loader_retrieve(load_balancer *main, char *key, int *server_id)
{
	unsigned int hash_key = hash_function_key(key);
	int pos = 0;

	pos = find_pos(main, hash_key);

	// In caz ca pos ajunge sa depaseasca dimensiunea
	pos %= main->hash_ring_size;

	*server_id = main->hash_ring[pos].label % TEN_TO_FIFTH;

	return (char *)server_retrieve(main->hash_ring[pos].server, key);
}

void free_load_balancer(load_balancer *main)
{
	for (unsigned int i = 0; i < main->hash_ring_size; i++)
		if (main->hash_ring[i].server)
			free_server_memory(main->hash_ring[i].server);

	free(main->hash_ring);

	free(main);
}
