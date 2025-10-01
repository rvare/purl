#define IDSIZE 1024
#define URLSIZE 2048

typedef struct reference {
	char id[IDSIZE];
	char url[URLSIZE];
} reference;

typedef struct reference_array {
	reference *arr;
	int size;
} reference_array;
