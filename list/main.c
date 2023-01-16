#include "list.h"

#include <string.h>
#include <stdio.h>

int main() {
	uint8_t bits = 74;
	float ff = 0.74f;
	char s[] = "This is a string";
	list_t* l = list_alloc();
	list_add(l, &bits, sizeof(uint8_t));
	list_add(l, &ff, sizeof(float));
	list_add(l, s, strlen(s) + 1);
	printf("element count: %lld\n", l->count);
	uint64_t size;
	void* ptr; 
	ptr = list_pop(l, &size);
	printf("%s\n", (char*)ptr);
	ptr = list_pop(l, &size);
	printf("%f\n", *((float*)ptr));
	ptr = list_pop(l, &size);
	printf("%d\n", *((uint8_t*)ptr));
	printf("element count: %lld\n", l->count);

	return 0;
}
