#ifndef LIST_H
#define LIST_H

#include <stdint.h>

typedef struct list_node_t {
	struct list_node_t* next;
	uint64_t data_size;
	void* data;
} list_node_t;

typedef struct {
	list_node_t* head;
	uint64_t count;
} list_t;

list_t*	list_alloc();
void 	list_free(list_t* l);

void*	list_get(const list_t* l, uint64_t index, uint64_t* data_size);
void 	list_set(const list_t* l, uint64_t index, const void* data, uint64_t data_size);
void	list_add(list_t* l, const void* data, uint64_t data_size);
void*	list_pop(list_t* l, uint64_t* data_size);

#endif
