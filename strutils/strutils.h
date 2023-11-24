#ifndef STRUTILS_H
#define STRUTILS_H

#include <stdint.h>
#include <stdio.h>

// HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
// All strings are assumed to be '\0' terminated
// HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH

/* 
	Split string into multiple substrings whenever a delimiter is found.

	[src] = string to be split
	[delim] = string containing all delimiters
	[str_count] = stores number of substrings generated (OUT)

	Returns an array containing all substrings found. 
	If no delimiter was found, the array contains a copy of the source string.
*/
char** cup_str_split(char* src, char* delim, size_t* strCount);

/* 
	Split string into multiple substrings whenever an exact match
	of the delimiter substring is found.

	[src] = string to be split
	[substr] = string containing the delimiter substring
	[str_count] = stores number of substrings generated (OUT)

	Returns an array containing all substrings found. 
	If no match was found, the array contains a copy of the source string.
*/
char** cup_str_split_substr(char* src, char* substr, size_t* str_count);

/*
	Count how many times a character was found in a string

	[src] = string on which to search
	[ch] = the target character

	Returns the number of times the character was found within the source string.
*/
size_t cup_count_char(char* src, char ch);

/*
	Count how many times a substring appears in a string

	[src] = string on which to search
	[substr] = the target substring

	Returns the number of times the substring was found within the source string.
*/
size_t cup_count_substr(char* src, char* substr);

/*
	Generate a subtring from a string

	[src] = source string to copy from
	[start] = position (index) from which to start copying
	[end] = position (index) on which to stop copying

	Returns the generated substring.
	If end is greater than the source's length, the substring ends at the source string's end.
	If start or end values are invalid (i.e., negative or end < start), returns NULL.	
*/
char* cup_substr(char* src, size_t start, size_t end);

/*
	Create a copy of a string

	[src] = string to be copied

	Returns the created copy.
	If memory allocation fails, returns NULL.
*/
char* cup_str_dup(char* src);

/*
	Transforms string to uppercase

	[src] = string to be transformed
*/
void cup_to_upper(char* src);

/*
 	Reads the entire file
  	
  	[in] = file descriptor of the file to be read
 
  	Returns a string with the file content.
	If memory allocation fails, returns NULL.
*/
char* cup_read_file(FILE* in);

#endif
