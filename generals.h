#pragma once

#include "load_balancer.h"

unsigned int hash_function_servers(void *a);

unsigned int hash_function_key(void *a);

void swap_data(hash_ring_t *a, hash_ring_t *b);

unsigned int get_server_id(load_balancer *main, int pos);

unsigned int get_hash(load_balancer *main, int pos);

/**
 *	Foloseste cautarea binara pentru a gasi pozitia potrivita unde se gaseste
 *	hash-ul dat ca parametru si returneaza pozitia unde se afla.
 *	@arg1: Load balancer-ul principal.
 *	@arg2: Hash-ul care trebuie gasit.
 */
int find_pos(load_balancer *main, unsigned int hash);

int find_dest_pos(load_balancer *main, int pos);
