#include "load_balancer.h"
#include "generals.h"

void rebalance_removing(load_balancer *main, unsigned int pos, int dest_pos)
{
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

				// Adaug in server-ul de la dest_pos si elimin din sursa.
				server_store(main->hash_ring[dest_pos].server,
							 (char *)data->key, (char *)data->value);

				server_remove(main->hash_ring[pos].server, (char *)data->key);
			} else {
				node = node->next;
			}
		}
		j++;
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

	exit(EXIT_FAILURE);
}
