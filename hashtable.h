/* Copyright 2023 Sampetru Mario */

#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAX_STRING_SIZE	256
#define HMAX 10

typedef unsigned int uint;

typedef struct ll_node_t {
	void *data;
	struct ll_node_t  *next;
} ll_node_t;

typedef struct linked_list_t {
	ll_node_t  *head;
	unsigned int data_size;
	unsigned int size;
} linked_list_t;

typedef struct hashtable_t hashtable_t;
struct hashtable_t {
	linked_list_t **buckets;
	unsigned int size;
	unsigned int hmax;
	unsigned int (*hash_function)(void *);
	int (*compare_function)(void *, void *);
	void (*key_val_free_function)(void *);
};

linked_list_t *ll_create(unsigned int data_size);

void ll_add_nth_node(linked_list_t *list, unsigned int n, const void
					 *new_data);

void ll_add_nth_node(linked_list_t *list, unsigned int n, const void
					 *new_data);

ll_node_t *ll_remove_nth_node(linked_list_t  *list, unsigned int n);

unsigned int ll_get_size(linked_list_t  *list);

void ll_free(linked_list_t **pp_list);

typedef struct info_t {
	void *key;
	void *value;
} info_t;

int compare_function_ints(void *a, void *b);

int compare_function_strings(void *a, void *b);

unsigned int hash_function_int(void *a);

unsigned int hash_function_string(void *a);

void key_val_free_function(void *data);

hashtable_t *ht_create(unsigned int hmax, unsigned int (*hash_function)(void *),
					   int (*compare_function)(void *, void *),
					   void (*key_val_free_function)(void *));

/**
 *	Aflu indexul, apoi parcurg lista in vederea gasirii cheii.
 *	@arg1: Dictionarul folosit pentru a cauta in el.
 *	@arg2: Cheia care trebuie gasita.
 */
int ht_has_key(hashtable_t *ht, void *key);

/**
 *	Aflu indexul, apoi parcurg lista in vederea gasirii cheii.
 *	Daca o gasesc o returnez, altfel returnez NULL.
 *	@arg1: Dictionarul folosit pentru a cauta in el.
 *	@arg2: Cheia care trebuie gasita.
 */
void *ht_get(hashtable_t *ht, void *key);

/**
 *	Aflu indexul, apoi parcurg lista in vederea gasirii listei in care
 *	trebuie pusa cheia. Verific daca aceasta exista deja. Daca da, actuali-
 *	zez doar valoarea, altfel pun si cheia si valoarea.
 *	@arg1: Dictionarul folosit pentru a cauta in el.
 *	@arg2: Cheia care trebuie pusa.
 *	@arg3: Lungimea cheii.
 *	@arg4: Valoarea care trebuie pusa.
 *	@arg5: Lungimea valorii.
 */
void ht_put(hashtable_t *ht, void *key, unsigned int key_size,
			void *value, unsigned int value_size);

/**
 *	Aflu indexul, apoi parcurg lista in vederea gasirii cheii.
 *	Daca o gasesc o elimin, altfel ma intorc fara sa fac nimic.
 *	@arg1: Dictionarul folosit pentru a cauta in el.
 *	@arg2: Cheia care trebuie gasita.
 */
void ht_remove_entry(hashtable_t *ht, void *key);

void ht_free(hashtable_t *ht);

#endif  // HASHTABLE_H_
