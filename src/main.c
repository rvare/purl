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

#define PATH_SIZE		512
#define CMD_VAL_OFFSET	3

typedef struct {
	char input_path[PATH_SIZE];
	char output_path[PATH_SIZE];
	char ref_path[PATH_SIZE];
} CmdArgs;

void parse_cmd_args(int, char **, CmdArgs *);
void show_help();
void show_version();

int main(int argc, char *argv[]) {
	if (argc <= 1) {
		fputs("Error: no input file or arguments given\n", stderr);
		return 1;
	}

	CmdArgs cmd_args = {0};

	parse_cmd_args(argc, argv, &cmd_args);

	FILE *input_file = NULL, *output_file = NULL;

	input_file = fopen(cmd_args.input_path, "r");
	if (input_file == NULL) {
		perror("Error with input file");
		return 1;
	}

	if (cmd_args.output_path[0] == '\0')
		output_file = stdout;
	else {
		if ((output_file = fopen(cmd_args.output_path, "w")) == NULL) {
			perror("Error with output file");
			return 1;
		}
	}

	reference_array ref_arr = load_reference(cmd_args.ref_path[0] == '\0' ? "./ref" : cmd_args.ref_path);

	register int token;
	while ((token=fgetc(input_file)) != EOF) {
		if (token == OPEN_BRACE) {
			token = fgetc(input_file);
			if (token == ID_INDICATOR)
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

void parse_cmd_args(int argc, char **argv, CmdArgs* cmd_args) {
	if (strcmp(argv[1], "--version") == 0) {
		show_version();
		exit(EXIT_SUCCESS);
	}
	else if (strcmp(argv[1], "--help") == 0) {
		show_help();
		exit(EXIT_SUCCESS);
	}

	if (argv[1][0] != '-') {
		strncpy(cmd_args->input_path, argv[1], PATH_SIZE);
		return;
	}

	for (size_t i = 1; i < argc; ++i) {
		if (argv[i][0] != '-') {
			fputs("Unrecognized argument\n", stderr);
			exit(EXIT_FAILURE);
		}

		switch(argv[i][1]) {
			case 'i':
				strncpy(cmd_args->input_path, argv[i]+CMD_VAL_OFFSET, PATH_SIZE);
				break;
			case 'o':
				strncpy(cmd_args->output_path, argv[i]+CMD_VAL_OFFSET, PATH_SIZE);
				break;
			case 'r':
				strncpy(cmd_args->ref_path, argv[i]+CMD_VAL_OFFSET, PATH_SIZE);
				break;
			default:
				fputs("Unrecognized argument\n", stderr);
				exit(EXIT_FAILURE);
				break;
		}
	}
}

void show_help() {
	puts(	"usage: purl [-i=<path>] [-r=<path>] [-o=<path>]\n"
			"   or: purl <input file path>\n"
			"\narguments:\n"
			"    -i         set path of input file\n"
			"    -r         set path of refernce file\n"
			"    -o         set path of output file\n"
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

// Comparator used to compare the reference ids with bsearch.
// This is used by process_id and load_reference. Putting this here is easier to share.
int id_comparator(const void *left, const void *right) {
	return strcmp(((reference*)left)->id, ((reference*)right)->id);
}
