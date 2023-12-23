#define CUP_STRUTILS_IMPLEMENTATION
#include "strutils.h"

#define OUTPUT stdout

typedef struct {
	char* in_str;
	char* in_delim;
	size_t out_count;
	char** out_strs;
} cup_str_split_test;

cup_str_split_test cup_str_split_tests[] = {
	{ 
		.in_str = "Do the impossible, see the invisible.", 
		.in_delim = " ", 
		.out_count = 6, 
		.out_strs = (char*[]) { "Do", "the", "impossible,", "see", "the", "invisible." } 
	},
	{
		.in_str = "Deixa eu baitar ele aqui!", 
		.in_delim = " e!", 
		.out_count = 10, 
		.out_strs = (char*[]) { "D", "ixa", "", "u", "baitar", "", "l", "", "aqui", "" }
	},
	{
		.in_str = "Nothing to see here, folks!", 
		.in_delim = "xyz?", 
		.out_count = 1, 
		.out_strs = (char*[]) { "Nothing to see here, folks!" } 
	},
	{
		.in_str = "", 
		.in_delim = "abc", 
		.out_count = 1, 
		.out_strs = (char*[]) { "" } 
	},
	{
		.in_str = "", 
		.in_delim = NULL, 
		.out_count = 1, 
		.out_strs = NULL 
	},
	{
		.in_str = NULL,
		.in_delim = "eiko", 
		.out_count = 1, 
		.out_strs = NULL
	}
};

uint8_t str_split_test(cup_str_split_test* test)
{
	// Test start
	size_t count;
	char** strings = cup_str_split(test->in_str, test->in_delim, &count);
	if (!test->in_delim || !test->in_str) {
		if (strings) {
			fprintf(OUTPUT, "FAILED: expected strings = NULL, got %p\n", strings);
			return 0;
		}
	}
	else if (!strings) {
		fprintf(OUTPUT, "FAILED: str_split returned NULL\n"); 
		return 0;
	}
	else if (count != test->out_count) {
		fprintf(OUTPUT, "FAILED: expected count = %lld, got %lld\n", test->out_count, count);
		return 0;
	}
	else {
		for (size_t i = 0; i < count; ++i) {
			if (strcmp(strings[i], test->out_strs[i])) {
				fprintf(OUTPUT, "FAILED: strings[%lld] = %s, got %s", i, test->out_strs[i], strings[i]); 
				return 0;
			}	
		}			
	}
	fprintf(OUTPUT, "PASSED\n");
	return 1;
}

int main()
{
	// cup_str_split
	fprintf(OUTPUT, ">> str_split\n");
	{
		for (size_t i = 0; i < sizeof(cup_str_split_tests) / sizeof(cup_str_split_test); ++i)
			str_split_test(&cup_str_split_tests[i]);
	}

	return 0;
}
