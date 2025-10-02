#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#include "../include/url_preprocessor.h"

int binary_search(char *id, reference *arr, int size);

void process_macro(FILE *input_file, FILE *output_file, reference_array *ref_arr) {
	assert(input_file != NULL);
	assert(output_file != NULL);
	assert(ref_arr != NULL);

	int token, i;
	char id[IDSIZE];
	char replacement[URLSIZE];

	for (i = 0, token=fgetc(input_file); i < IDSIZE-1 && token != '|' && token != EOF; ++i, token=fgetc(input_file))
		id[i] = token;
	id[i] = '\0';

	if (ferror(input_file)) {
		fprintf(stderr, "Error with input file");
		exit(1);
	}

	int index = binary_search(id, ref_arr->arr, ref_arr->size);
	if (index == -1) {
		fprintf(stderr, "Error: could not find id %s in reference file", id);
		exit(1);
	}
	
	for (i = 0, token=fgetc(input_file); i < URLSIZE-1 && token != '}' && token != EOF; ++i, token=fgetc(input_file))
		replacement[i] = token;
	replacement[i] = '\0';

	if (ferror(output_file)) {
		fprintf(stderr, "Error with output file");
		exit(1);
	}
	
	printf("in process_macro: %s %s\n%s\n", id, replacement, ref_arr->arr[index].url);

	fprintf(output_file, "[%s](%s)", replacement, ref_arr->arr[index].url);
}

/* binary_search: performs binary search based on the id of the reference. */
int binary_search(char *id, reference *arr, int size) {
	int low = 0,
	high = size - 1,
	mid = 0;

	while (low <= high) {
		mid = (low+high) / 2;
		if (strcmp(id, arr[mid].id) < 0)
			high = mid + 1;
		else if (strcmp(id, arr[mid].id) > 0)
			low = mid + 1;
		else
			return mid;
	}

	return -1;
}
