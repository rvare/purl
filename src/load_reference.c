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

void open_reference_file();
int get_token();

int token;
FILE *ref_file = NULL;

void load_reference() {
	int count = 0, tok_index = 0, arr_index = 0;
	reference *ref_arr;

	open_reference_file();

	while (get_token() != EOF)
		if (token == '%')
			++count;
	
	printf("%d\n", count);
	ref_arr = (reference*)calloc(count, sizeof(reference));

	rewind(ref_file);

	while (get_token() != EOF) {
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
			while (get_token() != EOF && token != '}')
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

/* open_reference: opens the reference file and checks for error. */
void open_reference_file() {
	ref_file = fopen("./ref", "r");
	if (ref_file == NULL) {
		perror("Error with reference file");
		exit(1);
	}
}

/* get_token: Helps simplify the the while loop. */
int get_token() {
	token = fgetc(ref_file);
	return token;
}
