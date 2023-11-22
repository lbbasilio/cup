#include "strutils.h"

#include <string.h>
#include <stdlib.h>

char** cup_str_split (char* src, char* delim, int* strCount)
{
	*strCount = 0;
	char** strings = NULL;
	if (src && delim)
	{
		int srcLength = strlen(src);
		if (!srcLength) return strings;

		int delimLength = strlen(delim);
		int count = 0;
		
		// Return if no delimiters found
		if (cup_str_find_first_of (src, delim, 0) == -1) 
		{
			strings = malloc(sizeof(char*));
			strings[0] = cup_str_dup (src);
			*strCount = 1;
			return strings;
		}

		// Count delimiter occurrences	
		int i;
		for (i = 0; i < delimLength; ++i) 
		{
			count += cup_count_char(src, delim[i]);
			if (src[srcLength - 1] == delim[i] && srcLength > 1) count--;
			if (src[0] == delim[i]) count--;
		} 

		// Ignore double delimiter occurrence
		char lastCh = src[0];
		for (i = 1; i < srcLength; ++i) 
		{
			char ch = src[i];
			int flag = 0;
			int j;
			for (j = 0; j < delimLength; ++j) {
				if (lastCh == delim[j]) flag++;
				if (ch == delim[j]) flag++;
			}

			if (flag == 2) count--;
			lastCh = ch;
		}

		// Allocate correct number of substrings
		strings = malloc((count + 1) * sizeof(char*));
		*strCount = count + 1;

		// Find and create substrings
		int pos = 0;
		int lastPos;
		for (i = 0; i <= count; ++i) 
		{
			lastPos = cup_str_find_first_not_of (src, delim, pos);
			pos = cup_str_find_first_of (src, delim, lastPos);
			if (pos == -1) pos = srcLength;

			strings[i] = cup_substr (src, lastPos, pos);
			pos++;
		}
	}
	return strings;
}

int cup_count_char (char* src, char ch) 
{
	int count = 0;
	if (src) {
		int i, length = strlen(src);
		for (i = 0; i < length; ++i)
			if (src[i] == ch) count++;
	}
	return count;
}

int cup_str_find_first_of (char* src, char* targets, int start)
{
	// Guard clauses
	if (!src)		return -1;
	if (!targets)	return -1; 
	if (start < 0)	return -1;

	int64_t src_len = (int64_t)strlen(src);
	if (start > src_len - 1) return -1;

	int64_t tgt_len = (int64_t)strlen(targets);
	if (!tgt_len) return -1;

	// Search for targets on input string
	for (int64_t position = start; position < src_len; ++position) {
		char ch = src[position];
		for (int64_t it = 0; it < tgt_len; ++it)
			if (ch == targets[it]) 
				return position;	
	}
	return -1;
}

int cup_str_find_first_not_of (char* src, char* targets, int start)
{	
	// Guard clauses
	if (!src)		return -1;
	if (!targets)	return -1;

	int64_t src_len = (int64_t)strlen(src);
	if (start > src_len - 1) return -1;

	int64_t tgt_len = (int64_t)strlen(targets);
	if (!tgt_len) return -1;

	// Look for first equality with targets
	for (int64_t position = start; position < src_len; ++position) {
		char ch = src[position];
		uint8_t flag = 0;
		for (int64_t it = 0; it < tgt_len; ++it) {
			flag += ch != targets[it];
		}
	}
	return -1;
}

char* cup_substr (char* src, int start, int end) 
{
	// Gaurd clauses
	if (!src) 			return NULL;
	if (end < 0) 		return NULL;
	if (start < 0) 		return NULL;
	if (start >= end) 	return NULL;

	// Allocate enough memory
	int64_t len = (int64_t)strlen(src);
	if (end > len) end = len;
	
	int64_t char_count = end - start;
	char* new_str = malloc(sizeof(char) * (char_count + 1));
	if (!new_str) return NULL;

	// Copy string
	memcpy((void*) new_str, (void*) (src + start), char_count);
	new_str[char_count] = '\0';

	return new_str;
}

char* cup_str_dup (char* src)
{
	char* copy = NULL;
	if (src)
	{
		unsigned len = strlen(src);
		if (len)
		{
			len++;
			copy = malloc(len * sizeof(char));
			if (copy)
				memcpy ((void*) copy, (void*) src, len);
		}
	}
	return copy;
}

void cup_to_upper (char* src)
{
	if (src)	
	{
		int i, len = strlen(src);
		for (i = 0; i < len; ++i)
		{
			char ch = src[i];
			if (ch > 0x60 && ch < 0x7B)
				src[i] = ch - 0x20;
		}
	}
}

void cup_str_cat (char* dst, char* src)
{
	if (dst && src)
	{
		int dstLen = strlen(dst);
		int srcLen = strlen(src);
		int i;
		for (i = 0; i < srcLen; ++i) dst[dstLen + i] = src[i];
		dst[dstLen + srcLen] = '\0';
	}
}

char* cup_read_file(FILE* in)
{
	fseek(in, 0, SEEK_END);
	int size = ftell(in);
	fseek(in, 0, SEEK_SET);

	char* content = malloc(size + 1);
	if (content)
	{
		fread(content, size, 1, in);
		content[size] = 0;
	}

	return content;
}
