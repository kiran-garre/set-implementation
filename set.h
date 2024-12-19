#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#ifndef SET_H
#define SET_H

#define TABLESIZE 53

typedef struct entry {
	void* key;
	struct entry* next;
} entry;

typedef struct set {
	int len;
	char* type;
	size_t key_size;
	void* equals;
	entry* table[TABLESIZE];
    entry* tails[TABLESIZE];
} set;

set* init_set(char* type, size_t key_size);

int push(set* s, void* key);

bool contains(set* s, void* key);

void pop(set* s, void* key);

void set_union(set* target, set* source);

void free_set(set* s);

#endif 