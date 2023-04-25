#pragma once

#include "load_balancer.h"

unsigned int hash_function_servers(void *a);

unsigned int hash_function_key(void *a);

void swap_data(hash_ring_t *a, hash_ring_t *b);

unsigned int get_server_id(load_balancer *main, int pos);

unsigned int get_hash(load_balancer *main, int pos);

int find_pos(load_balancer *main, unsigned int hash);
