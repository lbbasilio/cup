#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "cup/hashtable/hashtable.h"
#include "cup/strutils/strutils.h"

int main() {
	char str[] = "Nothing here but us chickens.";
	hashtable_t* ht = hashtable_alloc();
	size_t token_count;
	char num_str[50];
	char** tokens = cup_str_split(str, " ", &token_count);
	for (size_t i = 0; i < token_count; ++i)
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
