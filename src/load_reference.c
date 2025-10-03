#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#include "../include/url_preprocessor.h"

void open_reference_file();
int get_token();
void grab_id(reference_array *);
void grab_url(reference_array *);

FILE *ref_file = NULL;
int g_token, g_arr_index = 0;

reference_array load_reference() {
	int count = 0;
	reference_array ref_arr;

	open_reference_file();

	while (get_token() != EOF)
		if (g_token == '%')
			++count;
	
	ref_arr.arr = (reference*)calloc(count, sizeof(reference));
	ref_arr.size = count;

	rewind(ref_file);

	while (get_token() != EOF) {
		if (g_token == '%') {
			grab_id(&ref_arr);
			assert(g_token == '{');
			if (g_token == '{')
				ungetc(g_token, ref_file);
		}
		else if (g_token == '{') {
			grab_url(&ref_arr);
			++g_arr_index;
		}
		else if (isspace(g_token))
			continue;
	}

	if (ferror(ref_file)) {
		fprintf(stderr, "Error with reference file\n");
		exit(1);
	}

	fclose(ref_file);

	return ref_arr;
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
	g_token = fgetc(ref_file);
	return g_token;
}

/* grab_id: Grab the ID from reference file. */
void grab_id(reference_array *ref_arr) {
	int tok_index = 0;
	while (get_token() != EOF && g_token != '{')
		if (g_token != '\n') {
			ref_arr->arr[g_arr_index].id[tok_index] = g_token;
			++tok_index;
		}
	ref_arr->arr[g_arr_index].id[tok_index] = '\0';
}

/* grab_url: Grab the URL from reference file. */
void grab_url(reference_array *ref_arr) {
	int tok_index = 0;
	while (get_token() != EOF && g_token != '}')
		if (g_token != '\n') {
			ref_arr->arr[g_arr_index].url[tok_index] = g_token;
			++tok_index;
		}
	ref_arr->arr[g_arr_index].url[tok_index] = '\0';
}
