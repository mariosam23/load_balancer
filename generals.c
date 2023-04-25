#include "server.h"
#include "load_balancer.h"

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

void swap_data(hash_ring_t *a, hash_ring_t *b)
{
	hash_ring_t tmp = *a;
	*a = *b;
	*b = tmp;
}

unsigned int get_server_id(load_balancer *main, int pos)
{
	return main->hash_ring[pos].label % TEN_TO_FIFTH;
}

unsigned int get_hash(load_balancer *main, int pos)
{
	return hash_function_servers(&main->hash_ring[pos].label);
}

int find_pos(load_balancer *main, unsigned int hash)
{
	int pos = -1;
	int first_pos = 0;
	int last_pos = main->hash_ring_size - 1;

	if (hash <= get_hash(main, first_pos))
		return first_pos;

	if (hash >= get_hash(main, last_pos))
		return first_pos;

	while (first_pos <= last_pos) {
		int mid = (first_pos + last_pos) / 2;

		if (get_hash(main, mid) < hash && get_hash(main, mid + 1) > hash) {
			pos = mid + 1;
			break;
		} else if (get_hash(main, mid) < hash) {
			first_pos = mid + 1;
		} else if (get_hash(main, mid) > hash) {
			last_pos = mid - 1;
		}
	}

	return pos;
}
