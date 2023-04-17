/* Copyright 2023 Sampetru Mario */
#include "hashtable.h"
#include "utils.h"

linked_list_t *ll_create(unsigned int data_size)
{
    linked_list_t* ll;

    ll = malloc(sizeof(*ll));
	DIE(!ll, "malloc failed");

    ll->head = NULL;
    ll->data_size = data_size;
    ll->size = 0;

    return ll;
}

void ll_add_nth_node(linked_list_t* list, unsigned int n, const void* new_data)
{
    ll_node_t *prev, *curr;
    ll_node_t* new_node;

    if (!list) {
        return;
    }

    if (n > list->size) {
        n = list->size;
    }

    curr = list->head;
    prev = NULL;
    while (n > 0) {
        prev = curr;
        curr = curr->next;
        --n;
    }

    new_node = malloc(sizeof(*new_node));
    new_node->data = malloc(list->data_size);
    memcpy(new_node->data, new_data, list->data_size);

    new_node->next = curr;
    if (!prev) {
        list->head = new_node;
    } else {
        prev->next = new_node;
    }

    list->size++;
}

ll_node_t *ll_remove_nth_node(linked_list_t* list, unsigned int n)
{
    ll_node_t *prev, *curr;

    if (!list || !list->head) {
        return NULL;
    }

    if (n > list->size - 1) {
        n = list->size - 1;
    }

    curr = list->head;
    prev = NULL;
    while (n > 0) {
        prev = curr;
        curr = curr->next;
        --n;
    }

    if (!prev) {
        list->head = curr->next;
    } else {
        prev->next = curr->next;
    }

    list->size--;

    return curr;
}

unsigned int ll_get_size(linked_list_t* list)
{
     if (!list) {
        return -1;
    }

    return list->size;
}

void ll_free(linked_list_t** pp_list)
{
    ll_node_t* currNode;

    if (!pp_list || !*pp_list) {
        return;
    }

    while (ll_get_size(*pp_list) > 0) {
        currNode = ll_remove_nth_node(*pp_list, 0);
        free(currNode->data);
        currNode->data = NULL;
        free(currNode);
        currNode = NULL;
    }

    free(*pp_list);
    *pp_list = NULL;
}

/*
 * Functii de comparare a cheilor:
 */
int compare_function_ints(void *a, void *b)
{
	int int_a = *((int *)a);
	int int_b = *((int *)b);

	if (int_a == int_b) {
		return 0;
	} else if (int_a < int_b) {
		return -1;
	} else {
		return 1;
	}
}

int compare_function_strings(void *a, void *b)
{
	char *str_a = (char *)a;
	char *str_b = (char *)b;

	return strcmp(str_a, str_b);
}

/*
 * Functii de hashing:
 */
unsigned int hash_function_int(void *a)
{
	/*
	 * Credits: https://stackoverflow.com/a/12996028/7883884
	 */
	unsigned int uint_a = *((unsigned int *)a);

	uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
	uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
	uint_a = (uint_a >> 16u) ^ uint_a;
	return uint_a;
}

unsigned int hash_function_string(void *a)
{
	/*
	 * Credits: http://www.cse.yorku.ca/~oz/hash.html
	 */
	unsigned char *puchar_a = (unsigned char*) a;
	unsigned long hash = 5381;
	int c;

	while ((c = *puchar_a++))
		hash = ((hash << 5u) + hash) + c; /* hash * 33 + c */

	return hash;
}

void key_val_free_function(void *data)
{
    free(((info_t *)data)->key);
    free(((info_t *)data)->value);
}

hashtable_t *ht_create(unsigned int hmax, unsigned int (*hash_function)(void*),
		int (*compare_function)(void*, void*),
		void (*key_val_free_function)(void*))
{
	hashtable_t *ht = malloc(sizeof(*ht));
    DIE(!ht, "malloc failed");
	
	ht->hmax = hmax;
	ht->size = 0;

	ht->buckets = malloc(sizeof(*ht->buckets) * hmax);
	DIE(!ht->buckets, "malloc failed");

	for (uint i = 0; i < hmax; i++) {
		ht->buckets[i] = ll_create(sizeof(info_t));
	}

	ht->hash_function = hash_function;
	ht->compare_function = compare_function;
	ht->key_val_free_function = key_val_free_function;

	return ht;
}

int ht_has_key(hashtable_t *ht, void *key)
{	
	uint index = ht->hash_function(key) % ht->hmax;

	if (!ht->buckets[index])
		return 0;

	ll_node_t *find_node = ht->buckets[index]->head;

	while (find_node) {	
		if (ht->compare_function(key, ((info_t *)find_node->data)->key) == 0)
			return 1;

		find_node = find_node->next;
	}

	return 0;
}

void *ht_get(hashtable_t *ht, void *key)
{
	uint index = ht->hash_function(key) % ht->hmax;

	if (!ht->buckets[index])
		return NULL;

	ll_node_t *find_node = ht->buckets[index]->head;

	while (find_node) {
		if (ht->compare_function(key, ((info_t *)find_node->data)->key) == 0)
			return ((info_t *)find_node->data)->value;
		
		find_node = find_node->next;
	}

	return NULL;
}

void ht_put(hashtable_t *ht, void *key, unsigned int key_size,
	void *value, unsigned int value_size)
{
	uint index = ht->hash_function(key) % ht->hmax;

	linked_list_t *list = ht->buckets[index];

	ll_node_t *curr = list->head;

	int key_exists = 0;

	for (uint i = 0; i < ht->buckets[index]->size; i++) {
		if (ht->compare_function(key, ((info_t *)curr->data)->key) == 0) {
			key_exists = 1;
			break;
		}

		curr = curr->next;
	}	

	if (key_exists) {
		memcpy(((info_t *)curr->data)->value, value, value_size);
	} else {
		info_t data;

		data.value = malloc(value_size);
		DIE(!data.value, "malloc failed");

		data.key = malloc(key_size);
		DIE(!data.key, "malloc failed");

		memcpy(data.value, value, value_size);
		memcpy(data.key, key, key_size);

		ll_add_nth_node(list, 0, (const void *)&data);

		ht->size++;
	}
}

void ht_remove_entry(hashtable_t *ht, void *key)
{
	uint index = ht->hash_function(key) % ht->hmax;

	if (!ht->buckets[index])
		return;

	linked_list_t *list = ht->buckets[index];

	ll_node_t *curr = list->head;

	int pos = -1;

	for (uint i = 0; i < ht->buckets[index]->size; i++) {
		if (ht->compare_function(key, ((info_t *)curr->data)->key) == 0) {
			pos = i;
			break;
		}

		curr = curr->next;
	}

	if (pos == -1)
		return;

	ll_node_t *removed = ll_remove_nth_node(list, pos);

	key_val_free_function(((info_t *)removed->data));

	free(removed->data);
	free(removed);

	ht->size--;
}

void ht_free(hashtable_t *ht)
{	
    for (uint i = 0; i < ht->hmax; i++) {
		if (ht->buckets[i]) {
			ll_node_t *curr = ht->buckets[i]->head;

			while (curr) {
				key_val_free_function(curr->data);
				curr = curr->next;
			}
		}
		ll_free(&ht->buckets[i]);
	}

	free(ht->buckets);
	free(ht);
}
