#ifndef VECTOR_H
#define VECTOR_H

#include <stdint.h>

typedef struct {
	uint64_t count;	
	uint64_t limit;
	uint64_t element_size;
	void* elements;
} vector_t;

vector_t*	vector_alloc(uint64_t element_size, uint64_t init_limit);
void		vector_free(vector_t* v);

void*	vector_get(const vector_t* v, uint64_t index);
void	vector_set(const vector_t* v, uint64_t index, void* element);
void	vector_add(vector_t* v, void* element);
void*	vector_pop(vector_t* v);

#endif
