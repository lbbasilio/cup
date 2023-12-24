#ifndef CUP_VECTOR_H
#define CUP_VECTOR_H

#include <stdint.h>

typedef struct {
	uint64_t count;	
	uint64_t limit;
	uint64_t element_size;
	void* elements;
} vector_t;

vector_t* vector_alloc(uint64_t element_size, uint64_t init_limit);
void vector_free(vector_t* v);

void* vector_get(const vector_t* v, uint64_t index);
void vector_set(const vector_t* v, uint64_t index, void* element);
void vector_add(vector_t* v, void* element);
void* vector_pop(vector_t* v);

#endif


#ifdef CUP_VECTOR_IMPLEMENTATION

#include <stdlib.h>
#include <string.h>

vector_t* vector_alloc(uint64_t element_size, uint64_t init_limit)
{
	vector_t* v = malloc(sizeof(vector_t));	
	if (!v) return NULL;

	v->count = 0;
	v->limit = init_limit;
	v->element_size = element_size;
	v->elements = malloc(element_size * init_limit);
	if (!v->elements) {
		free(v);
		return NULL;
	}

	return v;
}

void vector_free(vector_t* v)
{
	if (!v) return;
	free(v->elements);
	free(v);
}

void* vector_get(const vector_t* v, uint64_t index)
{
	if (index >= v->count) return NULL;
	return ((uint8_t*)v->elements) + (v->element_size * index); 
}

void vector_set(const vector_t* v, uint64_t index, void* element)
{
	if (index >= v->count) return;
	void* pos = ((uint8_t*)v->elements) + (v->element_size * index);
	memcpy(pos, element, v->element_size);
}

void vector_add(vector_t* v, void* element)
{
	if (v->count == v->limit) {
		v->limit *= 2;
		v->elements = realloc(v->elements, v->limit);
	}

	void* pos = ((uint8_t*)v->elements) + (v->element_size * v->count);
	memcpy(pos, element, v->element_size);
	v->count++;
}

void* vector_pop(vector_t* v)
{
	if (!v->count) return NULL;
	v->count--;
	void* pos = ((uint8_t*)v->elements) + (v->element_size * (v->count));
	void* element = malloc(v->element_size);
	memcpy(element, pos, v->element_size);
	return element; 
}
#endif
