#include "vector.h"
#include <stdio.h>

int main() {
	uint32_t data[] = { 1, 2, 3, 4, 5 };
	vector_t* vec = vector_alloc(sizeof(uint32_t), 2);
	vector_add(vec, &data[0]);
	vector_add(vec, &data[1]);
	vector_add(vec, &data[2]);
	vector_add(vec, &data[3]);
	vector_add(vec, &data[4]);
	for (uint8_t i = 0; i < vec->count; i++)
		printf("%d\n", *(uint32_t*)vector_get(vec, i));

	vector_set(vec, 1, &data[3]);
	vector_set(vec, 2, &data[3]);
	for (uint8_t i = 0; i < vec->count; i++)
		printf("%d\n", *(uint32_t*)vector_get(vec, i));

	return 0;
}
