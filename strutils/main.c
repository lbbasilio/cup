#include "strutils.h"
#include <stdio.h>
#include <string.h>

#define OUTPUT stdout

uint8_t str_split_inner_test(char* delim, char* str, int64_t exp_count, char** exp_ans)
{
	// Test start
	int64_t count;
	char** strings = str_split(str, delim, &count);
	if (!delim || !str) {
		if (strings) {
			fprintf(OUTPUT, "FAILED: expected strings = NULL, got %p\n", strings);
			return 0;
		}
	}
	else if (!strings) {
		fprintf(OUTPUT, "FAILED: str_split returned NULL\n"); 
		return 0;
	}
	else if (count != exp_count) {
		fprintf(OUTPUT, "FAILED: expected count = %d, got %d\n", exp_count, count);
		return 0;
	}
	else {
		for (size_t i = 0; i < count; ++i) {
			if (strcmp(strings[i], exp_ans[i])) {
				fprintf(OUTPUT, "FAILED: strings[%d] = %s, got %s", i, exp_ans[i], strings[i]); 
				return 0;
			}	
		}			
	}
	fprintf(OUTPUT, "PASSED\n");
	return 1;
}

uint8_t str_split_test_1() 
{
	// Inputs
	char delim[] = " ";
	char str[] = "Do the impossible, see the invisible.";

	// Expected Outputs
	int64_t exp_count = 6;
	char* exp_ans[] = { "Do", "the", "impossible,", "see", "the", "invisible." }; 

	return str_split_inner_test(delim, str, exp_count, exp_ans);
}

uint8_t str_split_test_2() 
{
	// Inputs
	char delim[] = " e";
	char str[] = "Deixa eu baitar ele aqui ";

	// Expected Outputs
	int64_t exp_count = 6;
	char* exp_ans[] = { "D", "ixa", "u", "baitar", "l", "aqui" }; 

	return str_split_inner_test(delim, str, exp_count, exp_ans);
}

uint8_t str_split_test_3() 
{
	// Inputs
	char delim[] = "zxm?";
	char str[] = "Nothing to see here, folks!";

	// Expected Outputs
	int64_t exp_count = 1;
	char* exp_ans[] = { "Nothing to see here, folks!" }; 

	return str_split_inner_test(delim, str, exp_count, exp_ans);
}

uint8_t str_split_test_4() 
{
	// Inputs
	char delim[] = "abcd3";
	char str[] = "";

	// Expected Outputs
	int64_t exp_count = 1;
	char* exp_ans[] = { "" }; 

	return str_split_inner_test(delim, str, exp_count, exp_ans);
}

uint8_t str_split_test_5() 
{
	// Inputs
	char* delim = NULL;
	char str[] = "";

	// Expected Outputs
	int64_t exp_count = 1;
	char** exp_ans = NULL; 

	return str_split_inner_test(delim, str, exp_count, exp_ans);
}

uint8_t str_split_test_6() 
{
	// Inputs
	char delim[] = "eik0";
	char* str = NULL;

	// Expected Outputs
	int64_t exp_count = 1;
	char** exp_ans = NULL; 

	return str_split_inner_test(delim, str, exp_count, exp_ans);
}

uint8_t str_count_char_inner_test(char* src, char ch, int64_t exp_count)
{
	int64_t count = str_count_char(src, ch);
	if (count == exp_count) {
		fprintf(OUTPUT, "PASSED\n");
		return 1;
	}
	else {
		fprintf(OUTPUT, "FAILED: expected count = %d, got %d\n", exp_count, count);
		return 0;
	}
}

uint8_t str_count_char_test_1()
{
	char txt[] = "Shirazu shirazu kakushiteta hontou no koe wo hibikasete yo hora";
	char ch = 'a';
	int64_t count = 6;
	return str_count_char_inner_test(txt, ch, count);
}

uint8_t str_count_char_test_2()
{
	char txt[] = "Minai furi shiteitemo tashika ni soko ni aru, ima mo soko ni aru yo";
	char ch = 'i';
	int64_t count = 10;
	return str_count_char_inner_test(txt, ch, count);
}

int main()
{
	uint16_t current_passed = 0;
	uint16_t total_passed = 0;
	
	// str_split
	fprintf(OUTPUT, ">> str_split\n");
	{
		current_passed += str_split_test_1();
		current_passed += str_split_test_2();
		current_passed += str_split_test_3();
		current_passed += str_split_test_4();
		current_passed += str_split_test_5();
		current_passed += str_split_test_6();
	}

	fprintf(OUTPUT, ">> str_count_char\n");
	{
		current_passed += str_count_char_test_1();
		current_passed += str_count_char_test_2();
	}

	return 0;
}
