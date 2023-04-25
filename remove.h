#pragma once

#include "load_balancer.h"

/**
 *	Functia folosita pentru a rebalansa obiectele de pe serverele eliminate
 *	parcurgand, "galetile" din "dictionarul" server-ului proaspat eliminat
 *	si adaugand in serverul corespunzator.
 *	@arg1: Load balancer-ul principal.
 *	@arg2: Pozitia server-ului eliminat.
 *	@arg3: Pozitia server-ului ce o sa preia obiectele.
 */
void rebalance_removing(load_balancer *main, unsigned int pos, int dest_pos);

/**
 *	Gaseste pozitia server-ului ce trebuie eliminat din load balancer.
 *	@arg1: Load balancer-ul principal.
 *	@arg2: Hash-ul care trebuie gasit.
 */
int find_pos_to_remove(load_balancer *main, unsigned int hash_remv_serv);
