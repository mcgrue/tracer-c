// tracer-c.cpp : Defines the entry point for the console application.
//

#include "stdio.h"
#include "cJSON.h"
#include "cJSON_Utils.h"

#include "uthash.h" // http://troydhanson.github.io/uthash/

char** global_token_table = NULL; // put all unique strings here.  Then reference them by pointer in the tracery_particle.

typedef struct tracery_particle {
	char* word;     // "Taco"
	char* reroll;   // "#foo#"
	char* modifier; // ".capitalize" // probably a finite set of them.  Can they be chained?
	char* named;    // "[foo:#bar#]"

	UT_hash_handle hh;
};

// doesnt compile if uncommented
// typedef struct trace {
//	tracery_particle* line;
//};

// typedef grammar will be a hash of rerolls to => trace*'s

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

int main()
{
	char* raw_json_string = 0;

	printf("Hello, world\n\n");

	raw_json_string = load_file("unit_test.json");

	printf(raw_json_string);

	cJSON *obj = cJSON_Parse(raw_json_string);

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

	printf("before dump\n\n");

	// const cJSON *obj = cJSON_GetObjectItemCaseSensitive(parsed_json, "resolutions");;
	printf( cJSON_Print(obj) );

	printf("after dump\n\n");

end:
	printf("ending...");

	return 0;
}
