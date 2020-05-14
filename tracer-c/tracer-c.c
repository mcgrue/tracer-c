// tracer-c.cpp : Defines the entry point for the console application.
//

#include "assert.h"
#include "stdio.h"
#include "cJSON.h"
#include "cJSON_Utils.h"

#include "uthash.h" // http://troydhanson.github.io/uthash/

typedef struct {
	const char* word;
	UT_hash_handle hh;
} unique_word;

unique_word* unique_wordset = NULL;

/*
typedef struct {
	char* word;     // "Taco"
	char* reroll;   // "#foo#"
	char* modifier; // ".capitalize" // probably a finite set of them.  Can they be chained?
	char* named;    // "[foo:#bar#]"

	
} tracery_particle;

typedef struct {
	tracery_particle* key;
	tracery_particle** line;
	UT_hash_handle hh;
} trace;

typedef struct {

	UT_hash_handle hh;
} grammar;
*/

char* load_file(char* filename) {
	char* buffer = 0;
	long length;
	FILE * f = fopen(filename, "rb");

	if (f)
	{
		fseek(f, 0, SEEK_END);
		length = ftell(f);
		fseek(f, 0, SEEK_SET);
		buffer = calloc(1, length + 1);
		if (buffer)
		{
			fread(buffer, 1, length, f);
		}
		fclose(f);
	}

	return buffer;
}

/// adds a new word to unique_wordset if it's not already there.
void add_unique_word(char* word) {
	
	unique_word* uw;
	
	HASH_FIND_STR(unique_wordset, word, uw);  // id already in the hash?
	
	if( uw == NULL ) {
		uw = malloc(sizeof(unique_word));
		uw->word = word;
		HASH_ADD_KEYPTR(hh, unique_wordset, uw->word, strlen(uw->word), uw);
	}
}

void delete_all_unique_words() {
	unique_word* uw;
	unique_word* tmp;

	HASH_ITER(hh, unique_wordset, uw, tmp) {
		HASH_DEL(unique_wordset, uw);
		free(uw);
	}
}

char* print_all_unique_words() {
	unsigned int num_words;
	num_words = HASH_COUNT(unique_wordset);
	printf("there are %u words\n", num_words);

	unique_word* uw;

	char buffer[255] = "";

	for (uw = unique_wordset; uw != NULL; uw = uw->hh.next) {
		printf("%s,", uw->word);
		strcat(buffer, uw->word);
		strcat(buffer, ",");
	}

	return strdup(buffer);
}

int is_space(char c) {
	return (c == ' ' || c == '\t');
}

char* remove_leading_whitespaces(char* s) {
	while (is_space(s[0])) {
		++s;
	}

	return s;
}

/// this removes any line starting with // from the string.  Ignores leading whitespace
char* sanitize_string(char* original) {
	char* buffer = malloc(sizeof original);
	const char delim[2] = "\n";
	char *token;
	token = strtok(original, delim);

	while (token != NULL) {
		token = remove_leading_whitespaces(token);

		printf("TOKEN: %s\n", token);
		strcat(buffer, token);

		token = strtok(NULL, delim);
	}

	free(original);
	original = NULL;

	return buffer;
}


void do_json_stuff() {
	char* raw_json_string = 0;

	printf("Hello, world\n\n");

	raw_json_string = sanitize_string(load_file("unit_test.json"));

	//printf(raw_json_string);

	printf("Hello, world 2\n\n");

	cJSON *obj = cJSON_Parse(raw_json_string);

	printf("Hello, world 3\n\n");

	int status = 0;
	if (obj == NULL)
	{
		const char *error_ptr = cJSON_GetErrorPtr();
		if (error_ptr != NULL)
		{
			fprintf(stderr, "Error before: %s\n", error_ptr);
		}
		status = 0;
		goto end;
	}
	else
	{
		printf("We got something!\n\n");
	}

	printf("Hello, world 4\n\n");

	printf("before dump\n\n");

	// const cJSON *obj = cJSON_GetObjectItemCaseSensitive(parsed_json, "resolutions");;
	// printf(cJSON_Print(obj));

	const cJSON* key = NULL;
	const cJSON* val = NULL;

	cJSON_ArrayForEach(key, val) {
		// printf("'%s'=>'%s'\n", key, val);
	}


	printf("after dump\n\n");

end:
	printf("ending...");

	return 0;
}

int test_dictionary_uniqueness() {
	add_unique_word("taco");
	add_unique_word("burrito");
	add_unique_word("muffin");
	add_unique_word("jones");
	add_unique_word("burrito");
	add_unique_word("muffin");
	add_unique_word("alice");
	add_unique_word("taco");

	char* result = print_all_unique_words();
	char* expected = "taco,burrito,muffin,jones,alice,";

	int ret = 0;
	if ( strcmp(result, expected) == 0 ) {
		printf("test_dictionary_uniqueness PASSED!\n");
	}
	else {
		printf("test_dictionary_uniqueness FAILED\n!\n '%s' != '%s'", result, expected);
		ret = 1;
	}

	free(result); 
	result = NULL;
	delete_all_unique_words();

	return ret;
}

int test_remove_leading_whitespaces() {
	int ret = 0;

	char* expected = "TACO\t BURRITO";
	char* result = remove_leading_whitespaces(" \tTACO\t BURRITO");

	if ( strcmp(result, expected) == 0 ) {
		printf("remove_leading_whitespaces PASSED!\n");
	}
	else {
		printf("remove_leading_whitespaces FAILED\n!\n '%s' != '%s'", result, expected);
		ret = 1;
	}
	return ret;
}

void run_all_tests() {
	int fails = 0;

	fails += test_dictionary_uniqueness();
	fails += test_remove_leading_whitespaces();

	assert(fails == 0);
}

int main()
{
	run_all_tests();

	do_json_stuff();

	
	
	
	
	

	print_all_unique_words();

	printf("huah\n");
}
