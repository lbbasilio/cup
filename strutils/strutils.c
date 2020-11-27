#include "strutils.h"

#include <string.h>
#include <stdlib.h>

char** strSplit (char* src, char* delim, int* strCount)
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
		if (strFindFirstOf (src, delim, 0) == -1) 
		{
			strings = malloc(sizeof(char*));
			strings[0] = strDup (src);
			*strCount = 1;
			return strings;
		}

		// Count delimiter occurrences	
		int i;
		for (i = 0; i < delimLength; ++i) 
		{
			count += strCountChar(src, delim[i]);
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
			lastPos = strFindFirstNotOf (src, delim, pos);
			pos = strFindFirstOf (src, delim, lastPos);
			if (pos == -1) pos = srcLength;

			strings[i] = strSubstr (src, lastPos, pos);
			pos++;
		}
	}
	return strings;
}

int strCountChar (char* src, char ch) 
{
	int count = 0;
	if (src)
	{
		int i, length = strlen(src);
		for (i = 0; i < length; ++i) {
			if (src[i] == ch) count++;
		}
	}
	return count;
}

int strFindFirstOf (char* src, char* targets, int start)
{
	if (src)
	{
		int length = strlen(src);
		if (start > length - 1) return -1;

		int position;
		for (position = start; position < length; ++position) 
		{
			char ch = src[position];
			int num = strlen(targets);
			int it;
			for (it = 0; it < num; ++it) {
				if (ch == targets[it]) return position;	
			}
		}
	}
	return -1;
}

int strFindFirstNotOf (char* src, char* targets, int start)
{	
	if (src)
	{
		int length = strlen(src);
		if (start > length - 1) return -1;

		int position;
		for (position = start; position < length; ++position) 
		{
			char ch = src[position];
			int num = strlen(targets);
			int flag = 0;
			int it;
			for (it = 0; it < num; ++it) {
				if (ch != targets[it]) flag++;
			}

			if (flag == num) return position;
		}
	}
	return -1;
}

char* strSubstr (char* src, int start, int end) 
{
	char* newStr = NULL;
	if (src)
	{
		int length = strlen(src);
		if (start > length - 1 || start >= end || start < 0) return NULL;
		else 
		{
			if (end > length || end < 0) end = length;
			
			int charCount = end - start;
			newStr = malloc(sizeof(char) * (charCount + 1));
			if (newStr)
			{
				memcpy((void*) newStr, (void*) (src + start), charCount);
				newStr[charCount] = '\0';
			}
		}
	}
	return newStr;
}

char* strDup (char* src)
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

void strToUpper (char* src)
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

void strCat (char* dst, char* src)
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
