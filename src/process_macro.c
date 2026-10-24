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

int binary_search(char *id, reference *arr, int size);

void process_macro(FILE *input_file, FILE *output_file, reference_array *ref_arr) {
	register int token, i;
	char id[IDSIZE];

	// Get the ID
	for (	i = 0, token=fgetc(input_file);
			i < IDSIZE-1 && token != CLOSEBRACE && token != EOF;
			++i, token=fgetc(input_file)
		)
		id[i] = token;

	if (i < IDSIZE)
		id[i] = '\0';

	if (ferror(input_file)) {
		fputs("Error with input file\n", stderr);
		exit(EXIT_FAILURE);
	}

	// Find the ID for the URL
	register int index = binary_search(id, ref_arr->arr, ref_arr->size);
	if (index == -1) {
		fprintf(stderr, "Error: could not find id %s in reference file", id);
		exit(EXIT_FAILURE);
	}
	
	fprintf(output_file, "%s", ref_arr->arr[index].url);

	if (ferror(output_file)) {
		fputs("Error with output file\n", stderr);
		exit(EXIT_FAILURE);
	}
}

/* binary_search: performs binary search based on the id of the reference. */
int binary_search(char *id, reference *arr, int size) {
	register int low = 0,
	high = size - 1,
	mid = 0;

	while (low <= high) {
		mid = low + ((high-low) / 2);
		if (strcmp(id, arr[mid].id) < 0)
			high = mid - 1;
		else if (strcmp(id, arr[mid].id) > 0)
			low = mid + 1;
		else
			return mid;
	}

	return -1;
}
