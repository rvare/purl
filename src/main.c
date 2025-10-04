#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#include "../include/url_preprocessor.h"

int main(int argc, char *argv[]) {
	if (argc <= 1) {
		fprintf(stderr, "Error: no file given\n");
		return 1;
	}
	else if (argc <= 2) {
		fprintf(stderr, "Error: no destination given\n");
		return 1;
	}

	FILE *input_file = NULL,
	*output_file = NULL;

	int token;

	input_file = fopen(argv[1], "r");
	if (input_file == NULL) {
		perror("Error with input file");
		return 1;
	}

	output_file = fopen(argv[2], "w");
	if (output_file == NULL) {
		perror("Error with output file");
		return 1;
	}

	reference_array ref_arr = load_reference();
	assert(ref_arr.arr != NULL);
	assert(ref_arr.size > 0);

	while ((token=fgetc(input_file)) != EOF) {
		if (token == OPENBRACE) {
			token = fgetc(input_file);
			if (token == PERCENTSIGN) {
				printf("Call function\n");
				process_macro(input_file, output_file, &ref_arr);
			}
			else {
				assert(token != '%');
				fputc('{', output_file);
				fputc(token, output_file);
			}
		}
		else {
			assert(output_file != NULL);
			fputc(token, output_file);
		}
		assert(input_file != NULL);
	}

	if (ferror(input_file)) {
		fprintf(stderr, "Error with input file");
		return 1;
	}

	if (ferror(output_file)) {
		fprintf(stderr, "Error with output file");
		return 1;
	}

	fclose(input_file);
	fclose(output_file);

	return 0;
}
