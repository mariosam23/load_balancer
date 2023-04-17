#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "dll_list.h"

list_t*
dll_create(unsigned int data_size)
{
	list_t *list = malloc(sizeof(*list));
	DIE(!list, "Error allocating memory");

	list->data_size = data_size;
	list->size = 0;

	list->head = NULL;

	return list;
}

node_t*
dll_get_nth_node(list_t *list, unsigned long n)
{
	if (!list)
		return NULL;

	if (list->size == 0)
		return NULL;

	n = n % list->size;

	node_t *curr = list->head;

	for (unsigned int i = 0; i < n; i++)
		curr = curr->next;

	return curr;
}

void
dll_add_nth_node(list_t *list, unsigned long n, const void *new_data)
{
	if (!list)
		return;

	node_t *new = malloc(sizeof(*new));
	DIE(!new, "Error allocating memory");

	new->data = malloc(list->data_size);
	DIE(!new->data, "Error allocating memory");

	memcpy(new->data, new_data, list->data_size);

	new->next = NULL;
	new->prev = NULL;

	if (n >= list->size)
		n = list->size;

	node_t *prev;

	if (!list->head) {
		list->head = new;

		new->next = NULL;

		new->prev = NULL;
	} else if (n == 0) {
		new->next = list->head;

		list->head->prev = new;
		list->head = new;

		new->prev = NULL;
	} else {
		prev = dll_get_nth_node(list, n - 1);

		new->next = prev->next;
		new->prev = prev;

		if (prev->next)
			prev->next->prev = new;

		prev->next = new;
	}

	list->size++;
}

node_t*
dll_remove_nth_node(list_t *list, unsigned long n)
{
	node_t *removed;

	if (!list || !list->size)
		return NULL;

	if (n >= list->size)
		n = list->size - 1;

	removed = dll_get_nth_node(list, n);

	if (removed == list->head) {
		list->head = removed->next;
		if (list->head)
			list->head->prev = NULL;
	} else {
		removed->prev->next = removed->next;
		if (removed->next)
			removed->next->prev = removed->prev;
	}

	list->size--;

	if (!list->size)
		list->head = NULL;

	return removed;
}

void
dll_free(list_t **pp_list)
{
	node_t *node;

	if (!pp_list || !*pp_list)
		return;

	while ((*pp_list)->size) {
		node = dll_remove_nth_node(*pp_list, 0);

		free(node->data);
		free(node);
	}

	free(*pp_list);

	*pp_list = NULL;
}
