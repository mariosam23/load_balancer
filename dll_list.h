#pragma once

#define EXIT_CODE -1

#define DIE(condition, message) do {	\
	if (condition) {					\
		fprintf(stderr, message);		\
		exit(EXIT_CODE);				\
	}									\
} while (0)

typedef struct node_t node_t;

struct node_t {
	node_t *next;
	node_t *prev;
	void *data;
};

typedef struct {
	node_t *head;
	unsigned int size;
	unsigned int data_size;
} list_t;

// Se creaza si initializeaza o lista dublu inlantuita
list_t *dll_create(unsigned int data_size);

// Se obtine nodul dorit
node_t *dll_get_nth_node(list_t *list, unsigned long n);

// Se aloca memorie pentru noul nod, in care se copiaza noua informatie
// apoi, acesta este adaugat in lista la pozitia n.
void dll_add_nth_node(list_t *list, unsigned long n, const void *new_data);

// Se elimina nodul de la pozitia n
node_t *dll_remove_nth_node(list_t *list, unsigned long n);

// Se elibereaza toata memoria alocata pentru lista.
void dll_free(list_t **pp_list);
