#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdint.h>

typedef struct {
	char* key;
	void* data;
	uint64_t data_size;
} hashtable_node_t ;

typedef struct {
	vector_t* buckets[BUCKETS];
	uint64_t bucket_count;
} hashtable_t;

hashtable_t* hashtable_alloc();
void hashtable_free(hashtable_t* ht);
uint64_t hashtable_calc_hash(const char* key);
void* hashtable_get(const hashtable_t* ht, const char* key);
void  hashtable_set(const hashtable_t* ht, const char* key, const void* data, uint64_t data_size);

#endif
