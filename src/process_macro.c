#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#include "../include/url_preprocessor.h"

void process_macro(FILE *input_file, FILE *output_file) {
	int token, i;
	char id[IDSIZE];
	char replacement[URLSIZE];

	for (i = 0, token=fgetc(input_file); i < 1023 && token != '|' && token != EOF; ++i, token=fgetc(input_file))
		id[i] = token;
	id[i] = '\0';

	if (ferror(input_file)) {
		fprintf(stderr, "Error with input file");
		exit(1);
	}
	
	for (i = 0, token=fgetc(input_file); i < 1023 && token != '}' && token != EOF; ++i, token=fgetc(input_file))
		replacement[i] = token;
	replacement[i] = '\0';

	if (ferror(output_file)) {
		fprintf(stderr, "Error with output file");
		exit(1);
	}
	
	printf("%s %s\n", id, replacement);

	fprintf(output_file, "%s", replacement);
}

