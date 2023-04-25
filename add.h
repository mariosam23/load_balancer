#pragma once

#include "load_balancer.h"

/**
 *	Verifica daca este nevoie de realocare a hash_ring-ului din load balancer.
 *	@arg1: Load balancer-ul principal.
 */
void possible_realloc(load_balancer *main);

/**
 *	Adauga primul server in load_balancer.
 *	@arg1: Load balancer-ul principal.
 *	@arg2: Eticheta server-ului nou adaugat.
 *	@arg3: Noul server.
 */
void add_first_server(load_balancer *main, unsigned int label, server_memory *
					  new_server);

/**
 *	Functia ce rebalanseaza obiectele la adaugarea unui nou server.
 *	@arg1: Load balancer-ul principal.
 *	@arg2: Pozitia destinatiei.
 *	@arg3: Pozitia sursa.
 */
void rebalance_adding(load_balancer *main, int dest_pos, int src_pos);

/**
 *	Gaseste pozitia server-ului ce trebuie adaugat in load balancer.
 *	Daca doua servere au acelasi hash le compara dupa id.
 *	@arg1: Load balancer-ul principal.
 *	@arg2: Hash-ul server-ului care trebuie introdus.
 *	@arg3: Id-ul server-ului care trebuie introdus.
 */
int find_pos_to_add(load_balancer *main, unsigned int hash, unsigned int id);

/**
 *	Functia generala de adaugare a unui server in memorie, tinand cond de
 *	toate specificatiile.
 *	@arg1: Load balancer-ul principal.
 *	@arg2: Eticheta noului server.
 *	@arg3: Noul server deja alocat.
 */
void add_new_server(load_balancer *main, unsigned int label,
					server_memory *new_server);
