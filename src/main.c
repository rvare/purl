#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

int main() {
	FILE *ref_file = NULL,
	*input_file = NULL,
	*output_file = NULL;

	int token;

	ref_file = fopen("./ref", "r");
	if (ref_file == NULL) {
		perror("Error with reference file");
		return 1;
	}

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

	while ((token=fgetc(ref_file)) != EOF) {
		if (token == '%') {
			while ((token=fgetc(ref_file)) != EOF && token != '{') {
				printf("%c", token);
			}
			assert(token == '{');
			ungetc(token, ref_file);
		}
		else if (token == '{') {
			while ((token=fgetc(ref_file)) != EOF && token != '}') {
				printf("%c", token);
			}
		}
		else if (isspace(token)) {
			continue;
		}
	}

	if (ferror(ref_file)) {
		fprintf(stderr, "Error with reference file\n");
		return 1;
	}

	fclose(ref_file);

	while ((token=fgetc(input_file)) != EOF) {
		if (token == '{') {
			token = fgetc(input_file);
			if (token == '%')
				printf("Call function\n");
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
