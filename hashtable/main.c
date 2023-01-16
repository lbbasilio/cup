#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "strutils/strutils.h"
#include "vector/vector.h"

#define BUCKETS 11

typedef struct {
	char* key;
	void* data;
	uint64_t data_size;
} hashtable_node_t ;

typedef struct {
	vector_t* buckets[BUCKETS];
	uint64_t bucket_count;
} hashtable_t;

uint64_t hashtable_calc_hash(const char* key)
{
	uint64_t hash = 0;
	for(char* it = (char*)key; *it != 0; ++it)
		hash = *it + 31 * hash;
	return hash & BUCKETS;		
}

hashtable_t* hashtable_alloc()
{
	hashtable_t* ht = malloc(sizeof(hashtable_t));
	ht->bucket_count = BUCKETS;
	for (uint64_t i = 0; i < BUCKETS; ++i)
		ht->buckets[i] = vector_alloc(sizeof(hashtable_node_t), 10);
	return ht;
}

void* hashtable_get(const hashtable_t* ht, const char* key)
{
	uint64_t index = hashtable_calc_hash(key);
	vector_t* bucket = ht->buckets[index];
	for (uint64_t i = 0; i < bucket->count; ++i)
	{
		hashtable_node_t* node = vector_get(bucket, i);
		if (!strcmp(key, node->key))
			return node->data;
	}

	return NULL;
}

// When updating key values, you are responsible for freeing the previous object
void hashtable_set(hashtable_t* ht, const char* key, const void* data, uint64_t data_size)
{
	uint64_t index = hashtable_calc_hash(key);
	vector_t* bucket = ht->buckets[index];

	// Search for key
	for (uint64_t i = 0; i < bucket->count; ++i)
	{
		hashtable_node_t* node = vector_get(bucket, i);
		if (!strcmp(key, node->key)) {
			// Found key, replace data
			node->data_size = data_size;
			node->data = malloc(data_size);
			memcpy(node->data, data, data_size);
			return;
		}
	}

	// Add if not exists
	hashtable_node_t* new_node = malloc(sizeof(hashtable_node_t));	
	new_node->key = malloc(strlen(key));
	strcpy(new_node->key, key);

	new_node->data = malloc(data_size);
	new_node->data_size = data_size;
	memcpy(new_node->data, data, data_size);

	vector_add(bucket, new_node);
}

void hashtable_free(const hashtable_t* ht)
{
	for (uint64_t i = 0; i < ht->bucket_count; ++i)
		vector_free(ht->buckets[i]);
}

int main() {
	char str[] = "Nothing here but us chickens.";
	hashtable_t* ht = hashtable_alloc();
	int64_t token_count;
	char num_str[50];
	char** tokens = str_split(str, " ", &token_count);
	for (int64_t i = 0; i < token_count; ++i)
	{
		printf("%s\n", tokens[i]);
		hashtable_set(ht, itoa(i, num_str, 10), tokens[i], strlen(tokens[i]) + 1);
	}

	printf("%s\n", (char*)hashtable_get(ht, "0"));
	printf("%s\n", (char*)hashtable_get(ht, "3"));
	printf("%s\n", (char*)hashtable_get(ht, "1"));

	hashtable_set(ht, "0", str, strlen(str) + 1);
	printf("%s\n", (char*)hashtable_get(ht, "0"));

	return 0;
}
