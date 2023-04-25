#pragma once

#include "load_balancer.h"

void possible_realloc(load_balancer *main);

void add_first_server(load_balancer *main, unsigned int label, server_memory *
					  new_server);

void rebalance_adding(load_balancer *main, int dest_pos, int src_pos);

int find_pos_to_add(load_balancer *main, unsigned int hash, unsigned int id);

void add_new_server(load_balancer *main, unsigned int label,
					server_memory *new_server);
