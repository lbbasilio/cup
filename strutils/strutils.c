#include "strutils.h"

#include <string.h>
#include <stdlib.h>

char** cup_str_split(char* src, char* delim, size_t* str_count)
{
	*str_count = 0;
	char** strings = NULL;

	if (!src)	return strings;
	if (!delim)	return strings;

	size_t src_len = strlen(src);
	size_t delim_len = strlen(delim);
	
	// Return if no delimiters found
	if (!strpbrk(src, delim)) {
		strings = malloc(sizeof(char*));
		strings[0] = cup_str_dup(src);
		*str_count = 1;
		return strings;
	}

	// Count delimiter occurrences	
	size_t count = 0;
	for (size_t i = 0; i < src_len; ++i) {
		for (size_t j = 0; j < delim_len; ++j) {
			if (src[i] == delim[j]) {
				count++;
				break;
			}
		}
	}

	// Allocate correct number of substrings
	strings = malloc((count + 1) * sizeof(char*));
	*str_count = count + 1;

	// Find and create substrings
	for (size_t i = 0; i <= count; ++i) {
		size_t new_str_len = strcspn(src, delim);
		strings[i] = cup_substr(src, 0, new_str_len);
		src += new_str_len + 1;
	}

	return strings;
}

char** cup_str_split_substr(char* src, char* substr, size_t* str_count)
{
	*str_count = 0;
	char** strings = NULL;

	// Guard clauses
	if (!src) 		return strings;
	if (!substr)	return strings;
	
	size_t src_len = strlen(src);
	if (!src_len) return strings;

	size_t substr_len = strlen(substr);
	if (!substr_len) return strings;

	// Return if no substring was found
	if (!strstr(src, substr)) {
		strings = malloc(sizeof(char*));
		strings[0] = cup_str_dup(src);
		*str_count = 1;
		return strings;
	}

	// Count substring occurrences	
	//size_t count = 0;
	//for (size_t i = 0; i < substr_len; ++i) {
	//	count += cup_count_char(src, delim[i]);
	//	if (src[srcLength - 1] == delim[i] && srcLength > 1) count--;
	//	if (src[0] == delim[i]) count--;
	//} 

	//// Allocate correct number of substrings
	//strings = malloc((count + 1) * sizeof(char*));
	//*str_count = count + 1;

	//// Find and create substrings
	//size_t pos = 0;
	//for (i = 0; i <= count; ++i) {
	//	size_t last_pos = cup_str_find_first_not_of (src, delim, pos);
	//	pos = cup_str_find_first_of (src, delim, lastPos);
	//	if (pos == -1) pos = srcLength;

	//	strings[i] = cup_substr (src, lastPos, pos);
	//	pos++;
	//}

	return strings;
}

void cup_free_string_array(char** arr, size_t len)
{
	if (!arr) return;

	for (size_t i = 0; i < len; ++i)
		if (arr[i]) free(arr[i]);
	free(arr);
}

size_t cup_count_char(char* src, char ch) 
{
	if (!src) return 0;
	
	size_t count = 0;
	size_t length = strlen(src);
	for (size_t i = 0; i < length; ++i)
		if (src[i] == ch) count++;

	return count;
}

size_t cup_count_substr(char* src, char* substr)
{
	if (!src)		return 0;
	if (!substr)	return 0;

	size_t src_len = strlen(src);
	size_t substr_len = strlen(substr);
	if (!src_len) 				return 0;
	if (!substr_len) 			return 0;
	if (src_len < substr_len) 	return 0;

	size_t count = 0;
	while ((src = strstr(src, substr)))
		count++;

	return count;
}

char* cup_substr(char* src, size_t start, size_t end) 
{
	// Gaurd clauses
	if (!src) 			return NULL;
	if (start > end) 	return NULL;

	// Allocate enough memory
	size_t len = strlen(src);
	if (end > len) end = len;
	
	size_t char_count = end - start;
	char* new_str = malloc(sizeof(char) * (char_count + 1));
	if (!new_str) return NULL;

	// Copy string
	memcpy((void*)new_str, (void*)(src + start), char_count);
	new_str[char_count] = '\0';

	return new_str;
}

char* cup_str_dup(char* src)
{
	if (!src) return NULL;

	size_t len = strlen(src) + 1;
	char* copy = malloc(len * sizeof(char));
	if (copy) 
		memcpy ((void*) copy, (void*) src, len);

	return copy;
}

void cup_to_upper(char* src)
{
	if (!src) return;

	size_t len = strlen(src);
	for (size_t i = 0; i < len; ++i) {
		char ch = src[i];
		if (ch > 0x60 && ch < 0x7B)
			src[i] = ch - 0x20;
	}
}

char* cup_read_file(FILE* in)
{
	fseek(in, 0, SEEK_END);
	int size = ftell(in);
	fseek(in, 0, SEEK_SET);

	char* content = malloc(size + 1);
	if (!content) return NULL;

	fread(content, size, 1, in);
	content[size] = 0;

	return content;
}
