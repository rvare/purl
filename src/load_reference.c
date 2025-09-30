#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#define IDSIZE 1024
#define URLSIZE 2048

typedef struct reference {
	char id[IDSIZE];
	char url[URLSIZE];
} reference;

void load_reference() {
	FILE *ref_file = NULL;
	int token, count = 0, tok_index = 0,
	arr_index = 0;
	int i = 0, j = 0;
	reference *ref_arr;

	ref_file = fopen("./ref", "r");
	if (ref_file == NULL) {
		perror("Error with reference file");
		exit(1);
	}

	while ((token=fgetc(ref_file)) != EOF)
		if (token == '%')
			++count;
	
	printf("%d\n", count);
	ref_arr = (reference*)calloc(count, sizeof(reference));

	rewind(ref_file);

	while ((token=fgetc(ref_file)) != EOF) {
		if (token == '%') {
			while ((token=fgetc(ref_file)) != EOF && token != '{')
				if (token != '\n') {
					ref_arr[arr_index].id[tok_index] = token;
					++tok_index;
				}
			ref_arr[arr_index].id[tok_index] = '\0';
			assert(token == '{');
			if (token == '{')
				ungetc(token, ref_file);
		}
		else if (token == '{') {
			while ((token=fgetc(ref_file)) != EOF && token != '}')
				if (token != '\n') {
					ref_arr[arr_index].url[tok_index] = token;
					++tok_index;
				}
			ref_arr[arr_index].url[tok_index] = '\0';
			++arr_index;
		}
		else if (isspace(token))
			continue;
		tok_index = 0;
	}
}
