#include "strutils/strutils.h"
#include "hashtable/hashtable.h"
#include "list/list.h"
#include "json_parser/json_parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define ERROR printf("Could not parse json at position %lld ('%c') - state %d\n", it - json, *it, state) 

char str[] = "{\"A\":1,\"B\":\"ok\"}";
char strTest[] = "{\"Nothing\":\"chickens\",\"burnt\":\"nugget\",\"val\":1.875e+3, \"bo\":null} ";
char str2[] = "\"Bom dia meu nome eh Diego Brance e eu tenho 16 anos\"";
char str3[] = "12345 ";
char str4[] = "-789 ";
char str5[] = "3.1415 ";
char str6[] = "-14.79 ";
char str7[] = "0.345E+2 ";
char str8[] = "1576e-3 ";
char str9[] = "false";
char str10[] = "true";
char str11[] = "nul1";

uint8_t json_is_whitespace(const char c)
{
	const char whitespace[] = " \t\r\n";
	return strchr(whitespace, c) != NULL;
}

uint8_t json_is_escapable(const char c)
{
	const char escapable[] = "\"\\/bfnrt";
	return strchr(escapable, c) != NULL;
}

uint8_t json_is_hex_digit(const char c)
{
	const char hex_digits[] = "0123456789ABCDEFabcdef";
	return strchr(hex_digits, c) != NULL;
}

uint8_t json_is_dec_digit(const char c)
{
	const char dec_digits[] = "0123456789";
	return strchr(dec_digits, c) != NULL;
}

uint8_t json_is_pos_digit(const char c)
{
	const char pos_digits[] = "123456789";
	return strchr(pos_digits, c) != NULL;
}

uint8_t json_is_num_delim(const char c)
{
	const char num_delim[] = " \t\r\n,]}";
	return strchr(num_delim, c) != NULL;
}

char* parse_string(char* start, char** str, int64_t* size) 
{
	*str = NULL;
	*size = 0;
	enum string_machine_state { S0, S1, S2, S3, S4, S5, S6 };
	enum string_machine_state state = S0;
	char* it;
	for (it = start; *it != '\0'; ++it) {
		switch (state) {
			case S0:	if (*it == '\"') state = S1;
						else goto STR_END;
						break;
			case S1:	if (*it == '\\') state = S2;
						else if (*it == '\"') goto STR_SUCCESS;
						else state = S1;
						break;
			case S2:	if (*it == 'u') state = S3;
						else if (json_is_escapable(*it)) state = S1;
						else goto STR_END;
						break;
			case S3:	
			case S4:
			case S5:	if (json_is_hex_digit(*it)) state++;
						else goto STR_END;
						break;
			case S6:	if (json_is_hex_digit(*it)) state = S1;
						else goto STR_END;
						break;
		}
	}
	goto STR_END;

STR_SUCCESS:
	int64_t len = it - start;
	*str = malloc(len);
	memcpy(*str, start + 1, len - 1);
	(*str)[len-1] = '\0';
	*size = len;

STR_END:
	return ++it;
}

char* parse_number(char* start, void** num, int64_t* size)
{
	*num = NULL;
	*size = 0;
	enum number_machine_state { S0, S1, S2, S3, S4, S5, S6, S7, S8, S9 };
	enum number_machine_state state = S0;
	uint8_t is_float = 0;
	char* it;
	for (it = start; *it != '\0'; ++it) {
		switch (state) {
			case S0:	if (*it == '-') state = S1;
						else if (*it == '0') state = S2;
						else if (json_is_pos_digit(*it)) state = S3;
						else goto NUM_END;
						break;
			case S1:	if (*it == '0') state = S2;
						else if (json_is_pos_digit(*it)) state = S3;
						else goto NUM_END;
						break;
			case S2:	if (*it == '.') state = S5;
						else if (*it == 'e' || *it == 'E') state = S7;
						else if (json_is_num_delim(*it)) goto NUM_SUCCESS;
						else goto NUM_END;
						break;
			case S3:	if (*it == '.') state = S5;
						else if (*it == 'e' || *it == 'E') state = S7;
						else if (json_is_dec_digit(*it)) state = S4;
						else if (json_is_num_delim(*it)) goto NUM_SUCCESS;
						else goto NUM_END;
						break;
			case S4:	if (*it == '.') state = S5;
						else if (*it == 'e' || *it == 'E') state = S7;
						else if (json_is_dec_digit(*it)) state = S4;
						else if (json_is_num_delim(*it)) goto NUM_SUCCESS;
						else goto NUM_END;
						break;
			case S5:	is_float = 1;
						if (json_is_dec_digit(*it)) state = S6;
						else goto NUM_END;
						break;
			case S6:	if (*it == 'e' || *it == 'E') state = S7;
						else if (json_is_dec_digit(*it)) state = S6;
						else if (json_is_num_delim(*it)) goto NUM_SUCCESS;
						else goto NUM_END;
						break;
			case S7:	is_float = 1;
						if (*it == '-' || *it == '+') state = S8;
						else if (json_is_dec_digit(*it)) state = S9;
						else goto NUM_END;
						break;
			case S8:	if (json_is_dec_digit(*it)) state = S9;
						else goto NUM_END;
						break;
			case S9:	if (json_is_dec_digit(*it)) state = S9;
						else if (json_is_num_delim(*it)) goto NUM_SUCCESS;
						else goto NUM_END;
						break;
		}
	}

	goto NUM_END;

NUM_SUCCESS:
	int64_t len = it - start + 2;
	char* s = malloc(len);
	memcpy(s, start, len - 1);
	s[len-1] = '\0';
	if (is_float) {
		*num = malloc(sizeof(double));
		*((double*)*num) = atof(s);
		*size = sizeof(double);
	} else {
		*num = malloc(sizeof(int64_t));
		*((int64_t*)*num) = atoll(s);
		*size = sizeof(int64_t);
	}

NUM_END:
	return it;
}

char* parse_bool(char* start, int64_t** b, int64_t* size)
{
	*b = NULL;
	*size = 0;
	enum bool_machine_state { S0, S1, S2, S3, S4, S5, S6, S7 };
	enum bool_machine_state state = S0;
	uint8_t val = 0;
	char* it;
	for (it = start; *it != '\0'; ++it) {
		switch (state) {
			case S0:	if (*it == 't') state = S1;
						else if (*it == 'f') state = S4;
						else goto BOOL_END;
						break;
			case S1:	if (*it == 'r') state = S2;
						else goto BOOL_END;
						break;
			case S2:	if (*it == 'u') state = S3;
						else goto BOOL_END;
						break;
			case S3:	if (*it == 'e') {
							val = 1;
							goto BOOL_SUCCESS;
						}
						else goto BOOL_END;
						break;
			case S4:	if (*it == 'a') state = S5;
						else goto BOOL_END;
						break;
			case S5:	if (*it == 'l') state = S6;
						else goto BOOL_END;
						break;
			case S6:	if (*it == 's') state = S7;
						else goto BOOL_END;
						break;
			case S7:	if (*it == 'e') {
							val = 0;
							goto BOOL_SUCCESS;
						}
						else goto BOOL_END;
						break;
		}
	}
	
	goto BOOL_END;

BOOL_SUCCESS:
	*b = malloc(sizeof(int64_t));
	**b = (int64_t)val;
	*size = sizeof(int64_t);

BOOL_END:
	return it;
}

char* parse_null(char* start, void** ptr, int64_t* size)
{
	*ptr = (void*)UINTPTR_MAX;	// Assign non null value
	*size = sizeof(void*);
	enum null_machine_state { S0, S1, S2, S3 };
	enum null_machine_state state = S0;
	char* it;
	for (it = start; *it != '\0'; ++it) {
		switch (state) {
			case S0:	if (*it == 'n') state = S1;
						else goto NULL_END;
						break;
			case S1:	if (*it == 'u') state = S2;
						else goto NULL_END;
						break;
			case S2:	if (*it == 'l') state = S3;
						else goto NULL_END;
						break;
			case S3:	if (*it == 'l') goto NULL_SUCCESS;
						else goto NULL_END;
						break;
		}
	}
	goto NULL_END;

NULL_SUCCESS:
	*ptr = NULL;

NULL_END:
	return it;
}

char* parse_value(char* start, void** val, int64_t* size);

char* parse_object(char* start, hashtable_t** ht, int64_t* size)
{
	*ht = hashtable_alloc();
	*size = 0;
	enum object_machine_state { S0, S1, S2, S3, S4 };
	enum object_machine_state state = S0;
	char* current_key;
	char* it;
	for (it = start; *it != '\0'; ++it) {
		switch (state) {
			case S0:	if (*it == '{') state = S1;
						else goto OBJ_ERR;
						break;
			case S1:	if (*it == '}') goto OBJ_SUCCESS;
						else if (json_is_whitespace(*it)) state = S1;
						else if (*it == '\"') {
							int64_t size;
							it = parse_string(it, &current_key, &size);
							if (size) {
								--it;
								state = S2;
							}
							else goto OBJ_ERR;
						}
						break;
			case S2:	if (*it == ':') state = S3;
						else if (json_is_whitespace(*it)) state = S2;
						else goto OBJ_ERR;
						break;
			case S3:	if (json_is_whitespace(*it)) state = S3;
						else {
							void* val;
							int64_t size;
							it = parse_value(it, &val, &size);
							if (size) {
								--it;
								hashtable_set(*ht, current_key, val, size);
								current_key = NULL;
								state = S4;
							}
							else goto OBJ_ERR;
						}
						break;
			case S4:	if (*it == ',') state = S1;
						else if (*it == '}') goto OBJ_SUCCESS;
						else if (json_is_whitespace(*it)) state = S4;
						else goto OBJ_ERR;
		}
	}
OBJ_SUCCESS:
	*size = sizeof(hashtable_t);
	return ++it;
OBJ_ERR:
	hashtable_free(*ht);
	return it;
}

char* parse_array(char* start, list_t** l, int64_t* size)
{
	*l = list_alloc();
	*size = 0;
	enum array_machine_state { S0, S1, S2, S3 };
	enum array_machine_state state = S0;
	char* it;
	for (it = start; *it != '\0'; ++it) {
		int64_t el_size = 0;
		void* el_val = NULL;
		switch (state) {
			case S0:	if (*it == '[') state = S1;
						else goto ARR_END;
						break;
			case S1:	if (*it == ']') goto ARR_SUCCESS;
						else if (json_is_whitespace(*it)) state = S1;
						else {
							it = parse_value(it, &el_val, &el_size);
							if (el_size) {
								--it;
								list_add(*l, el_val, el_size);
								state = S2;
							}
							else goto ARR_ERR;
						}
						break;
			case S2:	if (*it == ',') state = S3;
						else if (*it == ']') goto ARR_SUCCESS;
						else if (json_is_whitespace(*it)) state = S2;
						else goto ARR_ERR;
						break;
			case S3:	if (json_is_whitespace(*it)) state = S3;
						else {
							it = parse_value(it, &el_val, &el_size);
							if (el_size) {
								--it;
								list_add(*l, el_val, el_size);
								state = S2;
							}
							else goto ARR_ERR;	
						}
		}
	}

ARR_SUCCESS:
	*size = sizeof(list_t);
	return it;

ARR_ERR:
	list_free(*l);
ARR_END:
	return it;
}

char* parse_value(char* start, void** val, int64_t* size)
{
	*val = NULL;
	*size = 0;

	if (*start == '\"') return parse_string(start, (char**)val, size);
	else if (*start == 't' || *start == 'f') return parse_bool(start, (int64_t**)val, size);
	else if (*start == '-' || json_is_dec_digit(*start)) return parse_number(start, val, size);
	else if (*start == 'n') return parse_null(start, val, size);
	else if (*start == '[') return parse_array(start, (list_t**)val, size); 
	else if (*start == '{') return parse_object(start, (hashtable_t**)val, size);

	return start;
}

// tokens: " : , } { 
enum parser_state { IDLE, S0, // Key start
	S1, S2, S3, S4, S5, S6, // String
	S7, // Key end
	S8, // Value start
	S9, S10, S11, S12, S13, S14, // String in value
	S15, // Value end
	S16, // Parse end
	S17, S18, S19, S20, S21, S22, S23, S24, S25, // Number in value
	S26, S27, S28, // True in value
	S29, S30, S31, S32, // False in value
	S33, S34, S35 // Null in value
};

hashtable_t* parse_json(char* json)
{
	hashtable_t* ht = hashtable_alloc();
	enum parser_state state = IDLE;
	uint8_t object_depth = 0;
	//uint8_t array_depth = 0;
	char* token_start = 0;
	char* current_key = NULL;
	for (char* it = json; *it != '\0'; ++it)
	{
		switch (state)
		{
			case IDLE:	if (*it == '{') { 
							object_depth++; 
							state = S0; 
						}
						break;
			case S0:	if (json_is_whitespace(*it)) state = S0;
						else if (*it == '\"') {
							token_start = it + 1;
							state = S1;						
						}
						else ERROR;	
						break;
			case S1:	if (*it == '\\') state = S2;
						else if (*it == '\"') {
							int64_t len = it - token_start + 1;
							current_key = malloc(len);
							memcpy(current_key, token_start, len - 1);
							current_key[len - 1] = '\0';
							state = S7;
						}
						else state = S1;
						break;
			case S2:	if (json_is_escapable(*it)) state = S1;
						else if (*it == 'u') state = S3;
						else ERROR;
						break;
			case S3:
			case S4:
			case S5:	if (json_is_hex_digit(*it)) state++;
						else ERROR;
						break;	
			case S6:	if (json_is_hex_digit(*it)) state = S1;
						else ERROR;
						break;	
			case S7:	if (json_is_whitespace(*it)) state = S7;
						else if (*it == ':') state = S8;
						else ERROR;
						break;
			case S8:	if (json_is_whitespace(*it)) state = S8;
						else if (*it == '\"') {
							token_start = it + 1;
							state = S9;	
						}
						else if (*it == '-') {
							token_start = it + 1;
							state = S17;
						}
						else if (*it == '0') { 
							token_start = it + 1;
							state = S18;
						}
						else if (json_is_pos_digit(*it)) {
							token_start = it + 1;
							state = S19;
						}
						else if (*it == 't') state = S26;
						else if (*it == 'f') state = S29;
						else if (*it == 'n') state = S33;
						else ERROR;
						break;
			case S9:	if (*it == '\\') state = S10;
						else if (*it == '\"') {
							int64_t len = it - token_start + 1;
							char* data = malloc(len);
							memcpy(data, token_start, len - 1);
							data[len - 1] = '\0';
							hashtable_set(ht, current_key, data, len);
							state = S15;
						}
						else state = S9;
						break;
			case S10:	if (json_is_escapable(*it)) state = S9;
						else if (*it == 'u') state = S11;
						else ERROR;
						break;
			case S11:
			case S12:
			case S13:	if (json_is_hex_digit(*it)) state++;
						else ERROR;
						break;
			case S14:	if (json_is_hex_digit(*it)) state = S9;
						else ERROR;
						break;
			case S15:	if (json_is_whitespace(*it)) state = S15;
						else if (*it == ',') state = S0;
						else if (*it == '}') state = S16;
						else ERROR;
						break;
			case S16:	printf("Success\n");
						break;
			case S17:	if (*it == '0') state = S18;
						else if (json_is_pos_digit(*it)) state = S19;
						else ERROR;
						break;
			case S18:	if (*it == '.') state = S21;
						else if (*it == 'e' || *it == 'E') state = S23;
						else if (*it == '}' || *it == ',' || json_is_whitespace(*it)) {
							int64_t len = it - token_start + 1;
							char* s = malloc(len);
							memcpy(s, token_start, len - 1);
							s[len - 1] = '\0';
							int64_t data = atoi(s);
							hashtable_set(ht, current_key, &data, sizeof(data));
							if (*it == '}') state = S16;
							else state = S0;
						}
						else if (*it == '}') state = S16;
						else ERROR;
						break;
			case S19:	if (*it == '.') state = S21;
						else if (*it == 'e' || *it == 'E') state = S23;
						else if (json_is_dec_digit(*it)) state = S20;
						else if (*it == '}' || *it == ',' || json_is_whitespace(*it)) {
							int64_t len = it - token_start + 1;
							char* s = malloc(len);
							memcpy(s, token_start, len - 1);
							s[len - 1] = '\0';
							int64_t data = atoi(s);
							hashtable_set(ht, current_key, &data, sizeof(data));
							if (*it == '}') state = S16;
							else state = S0;
						}
						else ERROR;
						break;
			case S20:	if (json_is_dec_digit(*it)) state = S20;
						else if (*it == '.') state = S21;
						else if (*it == 'e' || *it == 'E') state = S23;
						else if (*it == '}' || *it == ',' || json_is_whitespace(*it)) {
							int64_t len = it - token_start + 1;
							char* s = malloc(len);
							memcpy(s, token_start, len - 1);
							s[len - 1] = '\0';
							int64_t data = atoi(s);
							hashtable_set(ht, current_key, &data, sizeof(data));
							if (*it == '}') state = S16;
							else state = S0;
						}
						else ERROR;
						break;
			case S21:	if (json_is_dec_digit(*it)) state = S22;
						else ERROR;
						break;
			case S22:	if (json_is_dec_digit(*it)) state = S22;
						else if (*it == 'e' || *it == 'E') state = S23;
						else if (*it == '}' || *it == ',' || json_is_whitespace(*it)) {
							int64_t len = it - token_start + 1;
							char* s = malloc(len);
							memcpy(s, token_start, len - 1);
							s[len - 1] = '\0';
							double data = atof(s);
							hashtable_set(ht, current_key, &data, sizeof(data));
							if (*it == '}') state = S16;
							else state = S0;
						}
						else ERROR;
						break;
			case S23:	if (*it == '-' || *it == '+') state = S24;
						else if (json_is_dec_digit(*it)) state = S25;
						else ERROR;
						break;
			case S24:	if (json_is_dec_digit(*it)) state = S25;
						else ERROR;
						break;
			case S25:	if (json_is_dec_digit(*it)) state = S25;
						else if (*it == '}' || *it == ',' || json_is_whitespace(*it)) {
							int64_t len = it - token_start + 1;
							char* s = malloc(len);
							memcpy(s, token_start, len - 1);
							s[len - 1] = '\0';
							double data = atof(s);
							hashtable_set(ht, current_key, &data, sizeof(data));
							if (*it == '}') state = S16;
							else state = S0;
						}
						else ERROR;
						break;
			case S26:	if (*it == 'r') state = S27;
						else ERROR;
						break;
			case S27:	if (*it == 'u') state = S28;
						else ERROR;
						break;
			case S28:	if (*it == 'e') {
							uint8_t* data = malloc(sizeof(uint8_t));
							*data = 1;
							hashtable_set(ht, current_key, data, sizeof(uint8_t));
							state = S15;
						}
						else ERROR;
						break;
			case S29:	if (*it == 'a') state = S30;
						else ERROR;
						break;
			case S30:	if (*it == 'l') state = S31;
						else ERROR;
						break;
			case S31:	if (*it == 's') state = S32;
						else ERROR;
						break;
			case S32:	if (*it == 'e') {
							uint8_t* data = malloc(sizeof(uint8_t));
							*data = 0;
							hashtable_set(ht, current_key, data, sizeof(uint8_t));
							state = S15;
						}
						else ERROR;
						break;
			case S33:	if (*it == 'u') state = S34;
						else ERROR;
						break;
			case S34:	if (*it == 'l') state = S35;
						else ERROR;
						break;
			case S35:	if (*it == 'l') {
							uint64_t* data = malloc(sizeof(uint64_t));
							*data = 0;
							hashtable_set(ht, current_key, data, sizeof(uint64_t));
							state = S15;
						}
		}
	}

	return NULL;
}

//char str3[] = "12345";
//char str4[] = "-789";
//char str5[] = "3.1415";
//char str6[] = "-14.79";
//char str7[] = "0.345E+2";
//char str8[] = "1576e-3";

char str12[] = "[21,\"test\" , 42]";
char str13[] = "[21,[\"test\",42]]";
char str14[] = "{\"key\":\"value\", \"vvv\":42}";
char str15[] = "{\"key\":{\"value\":\"chika\", \"vvv\":426}, \"key22\":\"Kaguya\"}";

int main2()
{
	int64_t size;
	char* s;
	parse_string(str2, &s, &size);
	printf("%s\n", s);
	int64_t* v;
	parse_number(str3,(void**)&v, &size);
	printf("%lld\n", *v);
	free(v);
	parse_number(str4,(void**)&v, &size);
	printf("%lld\n", *v);
	free(v);
	double* d;
	parse_number(str5,(void**)&d, &size);
	printf("%lf\n", *d);
	free(d);
	parse_number(str6,(void**)&d, &size);
	printf("%lf\n", *d);
	free(d);
	parse_number(str7,(void**)&d, &size);
	printf("%lf\n", *d);
	free(d);
	parse_number(str8,(void**)&d, &size);
	printf("%lf\n", *d);
	free(d);

	parse_bool(str9, &v, &size);
	printf("%s\n", *v ? "true" : "false");
	free(v);
	parse_bool(str10, &v, &size);
	printf("%s\n", *v ? "true" : "false");
	free(v);

	void* ptr = (void*)0x10;
	parse_null(str11, &ptr, &size);
	printf("%p\n", ptr);

	list_t* l;
	parse_array(str12, &l, &size);
	printf("%lld\n", *((int64_t*)list_get(l, 0, &size)));
	printf("%s\n", (char*)list_get(l, 1, &size));
	printf("%lld\n", *((int64_t*)list_get(l, 2, &size)));
	list_free(l);

	parse_array(str13, &l, &size);
	printf("%lld\n", *((int64_t*)list_get(l, 0, &size)));
	printf("%s\n", (char*)list_get((list_t*)list_get(l, 1, &size), 0, &size));
	printf("%lld\n", *((int64_t*)list_get(list_get(l, 1, &size), 1, &size)));

	hashtable_t* ht;
	parse_object(str14, &ht, &size);
	printf("%s\n", (char*)hashtable_get(ht, "key"));
	printf("%lld\n", *(int64_t*)hashtable_get(ht, "vvv"));

	parse_object(str15, &ht, &size);
	printf("%s\n", (char*)hashtable_get(ht, "key22"));
	printf("%s\n", (char*)hashtable_get(hashtable_get(ht, "key"), "value"));
	printf("%lld\n", *(int64_t*)hashtable_get(hashtable_get(ht, "key"), "vvv"));
	return 0;
}

void hashtable_print(hashtable_t* ht)
{
	for (uint64_t i = 0; i < ht->bucket_count; ++i) {
		vector_t* v = ht->buckets[i];
		for (uint64_t j = 0; j < v->count; ++j) {
			hashtable_node_t* n = ((hashtable_node_t*)v->elements) + j;
			printf("ht[\"%s\"] has size %lld\n", n->key, n->data_size);
		}
	}

}

int main() 
{
	//hashtable_t* ht = json_parse(str15);
	//printf("%s\n", (char*)hashtable_get(ht, "key22"));
	//printf("%s\n", (char*)hashtable_get(hashtable_get(ht, "key"), "value"));
	//printf("%lld\n", *(int64_t*)hashtable_get(hashtable_get(ht, "key"), "vvv"));

	FILE* f = fopen("yt.json", "rb");
	char* json = NULL;
	fseek(f, 0, SEEK_END);
	long size = ftell(f);
	json = malloc(size + 1);
	fseek(f, 0, SEEK_SET);
	fread(json, sizeof(char), size, f);
	json[size - 1] = '\0';
	fclose(f);

	char* ptr = strstr(json, "streamingData");
	ptr = strchr(ptr, '{');
	char* it = ptr;
	int len = strlen(ptr);
	int depth = 0;
	do {
		if (*it == '{') depth++;
		if (*it == '}') depth--;
	} while (++it < ptr + len && depth > 0);

	f = fopen("out.json", "wb");
	fwrite(ptr, sizeof(char), it - ptr, f);
	hashtable_t* tt = json_parse(ptr);
	hashtable_print(tt);

	list_t* l = hashtable_get(tt, "adaptiveFormats");
	list_node_t* n = l->head;
	for (uint64_t i = 0; i < l->count; ++i, n = n->next) {
		hashtable_t* hh = (hashtable_t*)n->data;
		int64_t* itag = hashtable_get(hh, "itag");
		if (!itag) continue;
		printf("itag: %lld\n", *itag);
		char* mime = hashtable_get(hh, "mimeType");
		if (!mime) continue;
		printf("mime: %s\n", mime);
		if (!strstr(mime, "audio")) continue;
		char* quality = hashtable_get(hh, "audioQuality");
		if (!quality) continue;
		printf("audio quality: %s\n", quality);

		char* url = hashtable_get(hh, "url");
		char* cipher = hashtable_get(hh, "signatureCipher");
		if (url) {
			printf("Got url: %s\n", url);
		}
		else if (cipher) {
			char* s = strstr(cipher, "url=");
			if (!s) continue;
			url = s + 4;
			printf("Got url from signatureCipher: %s\n", url);
		}
	}	

	return 0;
}
