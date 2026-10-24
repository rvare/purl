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

void parse_cmd_args(char *);
void show_help();
void show_version();

int main(int argc, char *argv[]) {
	if (argc <= 1) {
		fputs("Error: no input file given or no arguments given\n", stderr);
		return 1;
	}

	if (argv[1][0] == '-')
		parse_cmd_args(argv[1]);

	if (argc <= 2) {
		fputs("Error: no destination file given\n", stderr);
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
				fputc('{', output_file);
				fputc(token, output_file);
			}
		}
		else
			fputc(token, output_file);
	}

	if (ferror(input_file)) {
		perror("Error with input file");
		return 1;
	}

	if (ferror(output_file)) {
		perror("Error with output file");
		return 1;
	}

	fclose(input_file);
	fclose(output_file);

	return 0;
}

void parse_cmd_args(char *args) {
	if (strcmp(args, "--version") == 0) {
		show_version();
		exit(EXIT_SUCCESS);
	}
	else if (strcmp(args, "--help") == 0) {
		show_help();
		exit(EXIT_SUCCESS);
	}
	else {
		fputs("Unrecognized argument\n", stderr);
		exit(EXIT_FAILURE);
	}
}

void show_help() {
	puts(	"purl [OPTIONS] [INPUT FILE] [OUTPUT FILE]"
			"    --help     display this help and exit\n"
			"    --version  display version and copyright information"
		);
}

void show_version() {
		puts(	"Purl v2.0\n"
				"Copyright (C) 2026 Richard Ernest Varela\n"
				"License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>\n"
				"This program comes with ABSOLUTELY NO WARRANTY.\n"
				"This is free software: you are free to change and redistribute it."
				);
}
