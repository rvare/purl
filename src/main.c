#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#include "../include/url_preprocessor.h"

int main() {
	FILE *input_file = NULL,
	*output_file = NULL;

	int token;

	input_file = fopen("./test.md", "r");
	if (input_file == NULL) {
		perror("Error with input file");
		return 1;
	}

	output_file = fopen("./out.md", "w");
	if (output_file == NULL) {
		perror("Error with output file");
		return 1;
	}

	reference_array ref_arr = load_reference();
	assert(ref_arr.arr != NULL);

	while ((token=fgetc(input_file)) != EOF) {
		if (token == '{') {
			token = fgetc(input_file);
			if (token == '%') {
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
