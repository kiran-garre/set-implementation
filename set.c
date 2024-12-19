#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "set.h"


/************************************************
 * HASHING FUNCTION                            
 ************************************************/
unsigned int fnv1a_hash(void* data, size_t key_size) {
    unsigned int hash = 2166136261u;  
    unsigned char* byte_data = (unsigned char*)data;

    for (size_t i = 0; i < key_size; ++i) {
        hash ^= byte_data[i];            
        hash *= 16777619u;               
    }

    return hash % TABLESIZE;
}

/************************************************
 * KEY COMPARISON FUNCTION                            
 ************************************************/
bool compare_keys(void* key1, void* key2, char* type, size_t key_size) {
	if (strcmp(type, "numeric") == 0) {
		return (memcmp(key1, key2, key_size) == 0);
	}	
	return (strcmp(key1, key2) == 0);
}

/************************************************
 * SET FUNCTIONS                            
 ************************************************/

/*
 * Allocates and returns an empty set
 */
set* init_set(char* type, size_t key_size) {
	set* s = malloc(sizeof(set));

	if (!(strcmp(type, "numeric") || strcmp(type, "string"))) {
		fprintf(stderr, "Unsupported type %s\n", type);
		exit(1);
	}
	if (strcmp(type, "numeric") && key_size == 0) {
		fprintf(stderr, "Key size must be specified for numeric types\n");
		exit(1);
	}
	
	s->len = 0;
	s->type = type;
	s->key_size = key_size;

	// Initializes arrays with sentinel nodes
	for (int i = 0; i < TABLESIZE; i++) {
		s->table[i] = malloc(sizeof(entry));
		s->table[i]->key = NULL;
		s->table[i]->next = NULL;
		s->tails[i] = s->table[i];
	}
	return s;
}

/*
 * Insert item into set 
 */
int push(set* s, void* key) {
	unsigned int index = fnv1a_hash(key, s->key_size);
    if (contains(s, key)) {
        return 1;
    }

	entry* new_entry = malloc(sizeof(entry));
	new_entry->key = key;
	new_entry->next = NULL;

	s->tails[index]->next = new_entry;
	s->tails[index] = new_entry;
    s->len++;
	return 0;
}

/*
 * Returns true if element is present in set
 */
bool contains(set* s, void* key) {
	unsigned int index = fnv1a_hash(key, s->key_size);

	entry* elem = s->table[index]->next;
	while (elem) {
		if (compare_keys(elem->key, key, s->type, s->key_size)) {
            return true;
		}
		elem = elem->next;
	}
	return false;
}

/*
 * Removes an element from the set if present
 */
void pop(set* s, void* key) {
	unsigned int index = fnv1a_hash(key, s->key_size);

	entry* elem = s->table[index]->next;
	entry* prev = s->table[index];
	while (elem) {
		if (compare_keys(elem->key, key, s->type, s->key_size)) {
			prev->next = elem->next;
			if (prev->next == NULL) {
				s->tails[index] = prev; // if element popped is last in list
			}
            free(elem);
            s->len--;
			return;
		}
		prev = elem;
		elem = elem->next;
	}
}

/*
 * Combines two sets; results are placed into target
 */
void set_union(set* target, set* source) {
    for (int i = 0; i < TABLESIZE; i++) {
        entry* curr = source->table[i]->next;
        while (curr) {
            push(target, curr->key);
			curr = curr->next;
        }
    }
}

/*
 * Frees all malloc'd memory 
 */
void free_set(set* s) {
    for (int i = 0; i < TABLESIZE; i++) {
        entry* temp;
        while (s->table[i] != NULL) {
            temp = s->table[i];
            s->table[i] = s->table[i]->next;
            free(temp);
        }
    }
    free(s);
    s = NULL;
}



