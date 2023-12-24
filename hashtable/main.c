#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define CUP_STRUTILS_IMPLEMENTATION
#include "cup/strutils/strutils.h"

#define CUP_HASHTABLE_IMPLEMENTATION
#include "hashtable.h"

int main() 
{
	char str[] = "Nothing here but us chickens.";
	hashtable_t* ht = hashtable_alloc();
	size_t token_count;
	char num_str[50];
	char** tokens = cup_str_split(str, " ", &token_count);
	for (size_t i = 0; i < token_count; ++i) {
		printf("%s\n", tokens[i]);
		snprintf(num_str, sizeof(num_str), "%d", i);
		hashtable_set(ht, num_str, tokens[i], strlen(tokens[i]) + 1);
	}

	printf("%s\n", (char*)hashtable_get(ht, "0"));
	printf("%s\n", (char*)hashtable_get(ht, "3"));
	printf("%s\n", (char*)hashtable_get(ht, "1"));

	hashtable_set(ht, "0", str, strlen(str) + 1);
	printf("%s\n", (char*)hashtable_get(ht, "0"));

	return 0;
}
