// tracer-c.cpp : Defines the entry point for the console application.
//

#include "stdio.h"
#include "cJSON.h"
#include "cJSON_Utils.h"

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

	raw_json_string = load_file("test.json");

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
