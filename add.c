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


int find_pos_to_add(load_balancer *main, unsigned int hash, unsigned int id)
{
	unsigned int pos = 0;
	while (pos < main->hash_ring_size && hash >=
			hash_function_servers(&main->hash_ring[pos].label)) {
		if (hash ==
			hash_function_servers(&main->hash_ring[pos].label)) {
			if (id % TEN_TO_FIFTH > main->hash_ring[pos].label
				% TEN_TO_FIFTH) {
				pos++;
				break;
			}
			break;
		}
		pos++;
	}

	return pos;
}

void add_new_server(load_balancer *main, unsigned int label,
					server_memory *new_server)
{
	unsigned int pos;
	pos = find_pos_to_add(main, hash_function_servers(&label),
						  label % TEN_TO_FIFTH);

	for (unsigned int j = main->hash_ring_size; j > pos; j--)
		swap_data(&main->hash_ring[j], &main->hash_ring[j - 1]);

	main->hash_ring_size++;
	main->hash_ring[pos].label = label;
	main->hash_ring[pos].server = new_server;

	int src_pos = pos + 1;
	while (src_pos < (int)main->hash_ring_size &&
		   get_server_id(main, src_pos) == get_server_id(main, pos)) {
		src_pos++;
	}

	src_pos %= main->hash_ring_size;

	if (main->hash_ring[src_pos].server->ht->size)
		rebalance_adding(main, pos, src_pos);
}
