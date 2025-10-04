#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#define IDSIZE		1024
#define URLSIZE		2048

#define IDINDICATOR	'%'
#define OPENBRACE	'{'
#define CLOSEBRACE	'}'

typedef struct reference {
	char id[IDSIZE];
	char url[URLSIZE];
} reference;

typedef struct reference_array {
	reference *arr;
	int size;
} reference_array;

reference_array load_reference();
void process_macro(FILE *, FILE *, reference_array *);
