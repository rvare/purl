/*
Purl a preprocessor for URL links.
Copyright (C) 2025  Richard Ernest Varela

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
#include <assert.h>

#include "../include/url_preprocessor.h"

int main(int argc, char *argv[]) {
	if (argc <= 1) {
		fprintf(stderr, "Error: no file given\n");
		return 1;
	}

	if (strcmp(argv[1], "--version") == 0) {
		printf("Purl v1.0\n"
				"Copyright (C) 2025 Richard Ernest Varela\n"
				"License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>\n"
				"This program comes with ABSOLUTELY NO WARRANTY.\n"
				"This is free software: you are free to change and redistribute it.\n"
				);
		return 0;
	}

	if (argc <= 2) {
		fprintf(stderr, "Error: no destination given\n");
		return 1;
	}

	FILE *input_file = NULL,
	*output_file = NULL;

	register int token;

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

	while ((token=fgetc(input_file)) != EOF) {
		if (token == OPENBRACE) {
			token = fgetc(input_file);
			if (token == IDINDICATOR)
				process_macro(input_file, output_file, &ref_arr);
			else {
				assert(token != IDINDICATOR);
				fputc('{', output_file);
				fputc(token, output_file);
			}
		}
		else
			fputc(token, output_file);
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
