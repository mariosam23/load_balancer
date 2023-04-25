#include "load_balancer.h"
#include "utils.h"
#include "generals.h"

void possible_realloc(load_balancer *main)
{
	if (main->hash_ring_capacity == main->hash_ring_size) {
		main->hash_ring_capacity *= INIT_SIZE;

		hash_ring_t *tmp = realloc(main->hash_ring, main->hash_ring_capacity *
								   sizeof(hash_ring_t));

		DIE(!tmp, "realloc faield");

		main->hash_ring = tmp;
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
				if (hash_function_key(data->key) <
					hash_function_servers(&main->hash_ring[dest_pos].label))
					server_store(main->hash_ring[dest_pos].server, data->key,
								 data->value);
				else if (!dest_pos)
					server_store(main->hash_ring[dest_pos].server, data->key,
								 data->value);
			}
			node = node->next;
		}
		j++;
	}
}

