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

unsigned int get_server_id(load_balancer *main, int pos)
{
	return main->hash_ring[pos].label % TEN_TO_FIFTH;
}

void rebalance(load_balancer *main, unsigned int pos, int dest_pos)
{
	// if (main->hash_ring[dest_pos].label % TEN_TO_FIFTH == 1)
	// 	printf("pos: %d     dest_pos %d\n\n", pos, dest_pos);
	if (!main->hash_ring[pos].server || !main->hash_ring[dest_pos].server)
		return;

	ll_node_t *node = NULL;
	unsigned int j = 0;

	while (j < main->hash_ring[pos].server->ht->hmax) {
		if (main->hash_ring[pos].server->ht->buckets[j]->size)
			node = main->hash_ring[pos].server->ht->buckets[j]->head;
		while (node) {
			info_t *data = NULL;
			if (node->data)
				data = (info_t *)node->data;
			if (data) {
				node = node->next;
				server_store(main->hash_ring[dest_pos].server, (char *)data->key, (char *)data->value);
				server_remove(main->hash_ring[pos].server, (char *)data->key);
			} else {
				node = node->next;
			}
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

void rebalance_adding(load_balancer *main, int dest_pos, int src_pos)
{
	if (!main->hash_ring[dest_pos].server || !main->hash_ring[src_pos].server)
		return;

	ll_node_t *node = NULL;
	unsigned int j = 0;

	while (j < main->hash_ring[src_pos].server->ht->hmax) {
		if (main->hash_ring[src_pos].server->ht->buckets[j]->head)
			node = main->hash_ring[src_pos].server->ht->buckets[j]->head;

		while (node) {
			info_t *data = NULL;
			if (node->data) {
				data = (info_t *)node->data;
				// printf("\nKEY: %u\nSERVER: %u\n", hash_function_key(data->key), hash_function_servers(&main->hash_ring[dest_pos].label));
				if (hash_function_key(data->key) < hash_function_servers(&main->hash_ring[dest_pos].label)) {
					server_store(main->hash_ring[dest_pos].server, data->key, data->value);

					// server_remove(main->hash_ring[src_pos].server, data->key);
				} else if (!dest_pos) {
					server_store(main->hash_ring[dest_pos].server, data->key, data->value);
				}
			}
			node = node->next;
		}
		j++;
	}
		// printf("\nSIZE: %d\n", main->hash_ring[src_pos].server->ht->size);

}

unsigned int get_hash(load_balancer *main, int pos)
{
	return hash_function_servers(&main->hash_ring[pos].label);
}

int find_pos(load_balancer *main, int pos, unsigned int hash_remv_serv, int id)
{
	pos = -1;
	int first_pos = 0;
	int last_pos = main->hash_ring_size - 1;

	while (first_pos <= last_pos) {
		int mij = (first_pos + last_pos) / 2;

		if (get_hash(main, mij) < hash_remv_serv && get_hash(main, mij + 1) > hash_remv_serv) {
			pos = mij + 1;
			break;
		} else if (get_hash(main, mij) == hash_remv_serv) {
			if ((int)get_server_id(main, mij) > id)
				pos = mij;
			else
				pos = mij + 1;
			break;
		} else if (get_hash(main, mij) < hash_remv_serv) {
			first_pos = mij + 1;
		} else if (get_hash(main, mij) > hash_remv_serv) {
			last_pos = mij - 1;
		}
	}

	return pos;
}

void add_new_server(load_balancer *main, unsigned int label, server_memory * new_server) {
	unsigned int pos = 0;

	while (pos < main->hash_ring_size && hash_function_servers(&label) >= hash_function_servers(&main->hash_ring[pos].label)) {
		if (hash_function_servers(&label) == hash_function_servers(&main->hash_ring[pos].label)) {
			if (label % TEN_TO_FIFTH > main->hash_ring->label % TEN_TO_FIFTH) {
				pos++;
				break;
			} else {
				break;
			}
		}
		pos++;
	}

	for (unsigned int j = main->hash_ring_size; j > pos; j--)
		swap_data(&main->hash_ring[j], &main->hash_ring[j - 1]);

	main->hash_ring_size++;
	main->hash_ring[pos].label = label;
	main->hash_ring[pos].server = new_server;

	int src_pos = pos + 1;
	while (src_pos < (int)main->hash_ring_size && get_server_id(main, src_pos) == get_server_id(main, pos)) {
		src_pos++;
	}

	src_pos %= main->hash_ring_size;

	if (main->hash_ring[src_pos].server->ht->size) {
		// printf("SIZE: %d\n\n", main->hash_ring[src_pos].server->ht->size);
		rebalance_adding(main, pos, src_pos);
	}
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

int find_pos_to_remove(load_balancer *main, unsigned int hash_remv_serv)
{
	int first_pos = 0;
	int last_pos = main->hash_ring_size - 1;

	while (first_pos <= last_pos) {
		int middle = (first_pos + last_pos) / 2;

		if (hash_remv_serv == get_hash(main, middle))
			return middle;
		else if (hash_remv_serv > get_hash(main, middle))
			first_pos = middle + 1;
		else
			last_pos = middle - 1;
	}

	printf("Something went wrong\n");
	exit(EXIT_FAILURE);
}


void loader_remove_server(load_balancer *main, int server_id)
{
	for (int i = 0; i < NR_REPLICAS; i++) {
		int pos = 0;
		unsigned int label = i * TEN_TO_FIFTH + server_id;
		pos = find_pos_to_remove(main, hash_function_servers(&label));

		int dest_pos;
		for (dest_pos = pos + 1; dest_pos < (int)main->hash_ring_size; dest_pos++)
			if (get_server_id(main, dest_pos) != get_server_id(main, pos))
				break;

		dest_pos %= main->hash_ring_size;

		if (main->hash_ring[pos].server->ht->size)
			rebalance(main, pos, dest_pos);

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
	int pos = 0;

	while (pos < (int)main->hash_ring_size &&
		   hash_function_servers(&main->hash_ring[pos].label) < hash_key)
		pos++;

	// In caz ca pos ajunge sa depaseasca dimensiunea
	pos %= main->hash_ring_size;

	*server_id = main->hash_ring[pos].label % TEN_TO_FIFTH;

	server_store(main->hash_ring[pos].server, key, value);
}

char *loader_retrieve(load_balancer *main, char *key, int *server_id)
{
	unsigned int hash_key = hash_function_key(key);
	int pos = 0;

	while (pos < (int)main->hash_ring_size &&
		   hash_function_servers(&main->hash_ring[pos].label) < hash_key)
		pos++;

	// In caz ca pos ajunge sa depaseasca dimensiunea
	pos %= main->hash_ring_size;

	*server_id = main->hash_ring[pos].label % TEN_TO_FIFTH;

	return (char *)server_retrieve(main->hash_ring[pos].server, key);
}

void free_load_balancer(load_balancer *main)
{
	for (unsigned int i = 0; i < main->hash_ring_size; i++) {
		// printf("hash_label :  0x%x     ,   server_id: %d\n", hash_function_servers(&main->hash_ring[i].label), main->hash_ring[i].label % TEN_TO_FIFTH);
		if (main->hash_ring[i].server)
			free_server_memory(main->hash_ring[i].server);
	}

	free(main->hash_ring);

	free(main);
}
