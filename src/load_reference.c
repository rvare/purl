#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#define IDSIZE 1024
#define URLSIZE 2048

struct reference {
	char id[IDSIZE];
	char url[URLSIZE];
};

void load_reference() {
	FILE *ref_file = NULL;
	int token, count = 0;

	ref_file = fopen("./ref", "r");
	if (ref_file == NULL) {
		perror("Error with reference file");
		exit(1);
	}

	while ((token=fgetc(ref_file)) != EOF)
		if (token == '%')
			++count;
	
	printf("%d\n", count);

	rewind(ref_file);

	while ((token=fgetc(ref_file)) != EOF) {
		if (token == '%') {
			while ((token=fgetc(ref_file)) != EOF && token != '{')
				if (token != '\n')
					printf("%c", token);
			assert(token == '{');
			if (token == '{')
				ungetc(token, ref_file);
		}
		else if (token == '{')
			while ((token=fgetc(ref_file)) != EOF && token != '}')
				if (token != '\n')
					printf("%c", token);
		else if (isspace(token))
			continue;
	}
}
