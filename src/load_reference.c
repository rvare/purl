/*
Purl a preprocessor for URL links.
Copyright (C) 2026  Richard Ernest Varela

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "../include/url_preprocessor.h"

void open_reference_file();
int get_token();
void grab_id(reference_array *);
void grab_url(reference_array *);

int g_token, g_arr_index = 0;
FILE *ref_file = NULL;

reference_array load_reference(char* ref_path) {
	register int count = 0;
	reference_array ref_arr;

	ref_file = fopen(ref_path, "r");
	if (ref_file == NULL) {
		perror("Error with reference file");
		exit(EXIT_FAILURE);
	}

	while (get_token() != EOF)
		if (g_token == ID_INDICATOR)
			++count;
	
	ref_arr.arr = (reference*)calloc(count, sizeof(reference));
	if (ref_arr.arr == NULL) {
		fputs("Error: Could not allocate memory for references.\n", stderr);
		exit(EXIT_FAILURE);
	}
	ref_arr.size = count;

	rewind(ref_file);

	while (get_token() != EOF) {
		if (g_token == ID_INDICATOR) {
			grab_id(&ref_arr);
			if (g_token == OPEN_BRACE)
				ungetc(g_token, ref_file);
		}
		else if (g_token == OPEN_BRACE) {
			grab_url(&ref_arr);
			++g_arr_index;
		}
		else if (isspace(g_token))
			continue;
	}

	if (ferror(ref_file)) {
		fputs("Error with reference file\n", stderr);
		exit(EXIT_FAILURE);
	}

	fclose(ref_file);

	return ref_arr;
}

/* get_token: Helps simplify the the while loop. */
int get_token() {
	return g_token = fgetc(ref_file);
}

/* grab_id: Grab the ID from reference file. */
void grab_id(reference_array *ref_arr) {
	register int tok_index = 0;
	while (get_token() != EOF && g_token != OPEN_BRACE)
		if (!isspace(g_token)) {
			ref_arr->arr[g_arr_index].id[tok_index] = g_token;
			++tok_index;
		}
	ref_arr->arr[g_arr_index].id[tok_index] = '\0';
}

/* grab_url: Grab the URL from reference file. */
void grab_url(reference_array *ref_arr) {
	register int tok_index = 0;
	while (get_token() != EOF && g_token != CLOSE_BRACE)
		if (!isspace(g_token)) {
			ref_arr->arr[g_arr_index].url[tok_index] = g_token;
			++tok_index;
		}
	ref_arr->arr[g_arr_index].url[tok_index] = '\0';
}
