#ifndef STRUTILS_H
#define STRUTILS_H

// HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
// All strings are assumed to be 0 terminated
// HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH

/* 
	Split string into multiple substrings whenever a delimiter is found.

	[src] = string to be split
	[delim] = string containing all delimiters
	[strCount] = stores number of substrings generated (OUT)

	Returns an array containing all substrings found. 
	If no delimiter was found, the array contains a copy of the source string.
*/
char** strSplit (char* src, char* delim, int* strCount);

/*
	Count how many times a character was found in a string

	[src] = string on which to search
	[ch] = the target character

	Returns the number of times the character was found within the source string.
*/
int strCountChar (char* src, char ch);

/*
	Find the first occurrence of one of a list of characters

	[src] = string on which to search
	[targets] = string containing target characters
	[start] = position (index) on which to start the search

	If one of the characters are found within the string, returns its position (index).
	If none were found, returns -1.
*/
int strFindFirstOf (char* src, char* targets, int start);

/*
	Find the first occurrence of a character different than a list of characters

	[src] = string on which to search
	[targets] = string containing target characters
	[start] = position (index) on which to start the search

	If one of the characters are found within the string, returns its position (index).
	If none were found or start is greater than the source's length, returns -1.
*/
int strFindFirstNotOf (char* src, char* targets, int start);

/*
	Generate a subtring from a string

	[src] = source string to copy from
	[start] = position (index) from which to start copying
	[end] = position (index) on which to stop copying

	Returns the generated substring.
	If end is greater than the source's length, the substring ends at the source string's end.
	If start or end values are invalid (i.e., negative or end < start), returns NULL.	
*/
char* strSubstr (char* src, int start, int end);

/*
	Create a copy of a string

	[src] = string to be copied

	Returns the created copy.
	If memory allocation fails, returns NULL.
*/
char* strDup (char* src);

/*
	Transforms string to uppercase

	[src] = string to be transformed
*/
void strToUpper (char* src);

/*
	Concatenate two strings

	[dst] = destination string
	[src] = source string

	After execution, [dst] contains the content of [dst] concatenated with [src].
	[dst] must be able to accomodate the whole [src] string + itself
*/
void strCat (char* dst, char* src);

#endif
