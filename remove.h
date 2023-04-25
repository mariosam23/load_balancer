#pragma once

#include "load_balancer.h"

void rebalance_removing(load_balancer *main, unsigned int pos, int dest_pos);

int find_pos_to_remove(load_balancer *main, unsigned int hash_remv_serv);
