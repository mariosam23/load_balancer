/* Copyright 2023 Sampetru Mario */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define DIE(assertion, call_description)				\
	do {								\
		if (assertion) {					\
			fprintf(stderr, "(%s, %d): ",			\
					__FILE__, __LINE__);		\
			perror(call_description);			\
			exit(errno);				        \
		}							\
	} while (0)

#define MAX_STRING_SIZE	256
#define HMAX 10

typedef unsigned int uint;

typedef struct ll_node_t
{
    void* data;
    struct ll_node_t* next;
} ll_node_t;

typedef struct linked_list_t
{
    ll_node_t* head;
    unsigned int data_size;
    unsigned int size;
} linked_list_t;

linked_list_t *ll_create(unsigned int data_size);

void ll_add_nth_node(linked_list_t* list, unsigned int n, const void* new_data);

void ll_add_nth_node(linked_list_t* list, unsigned int n, const void* new_data);

ll_node_t *ll_remove_nth_node(linked_list_t* list, unsigned int n);

unsigned int ll_get_size(linked_list_t* list);

void ll_free(linked_list_t** pp_list);

void ll_print_int(linked_list_t* list);

void ll_print_string(linked_list_t* list);

typedef struct info_t {
	void *key;
	void *value;
} info_t;

typedef struct hashtable_t hashtable_t;
struct hashtable_t {
	linked_list_t **buckets;
	unsigned int size;
	unsigned int hmax;
	unsigned int (*hash_function)(void*);
	int (*compare_function)(void*, void*);
	void (*key_val_free_function)(void*);
};

int compare_function_ints(void *a, void *b);

int compare_function_strings(void *a, void *b);

unsigned int hash_function_int(void *a);

unsigned int hash_function_string(void *a);

void key_val_free_function(void *data);

hashtable_t *ht_create(unsigned int hmax, unsigned int (*hash_function)(void*),
		int (*compare_function)(void*, void*),
		void (*key_val_free_function)(void*));

int ht_has_key(hashtable_t *ht, void *key);

void *ht_get(hashtable_t *ht, void *key);

void ht_put(hashtable_t *ht, void *key, unsigned int key_size,
	void *value, unsigned int value_size);

void ht_remove_entry(hashtable_t *ht, void *key);

void ht_free(hashtable_t *ht);

unsigned int ht_get_size(hashtable_t *ht);

unsigned int ht_get_hmax(hashtable_t *ht);
