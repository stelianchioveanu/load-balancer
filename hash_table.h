/* Copyright 2021 Chioveanu Stelian-Ovidiu */
#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_

#include "linked_list.h"

struct info {
	void *key;
	void *value;
};

typedef struct hashtable_t hashtable_t;
struct hashtable_t {
	linked_list_t **buckets;
	unsigned int size;
	unsigned int hmax;
	unsigned int (*hash_function)(void*);
	int (*compare_function)(void*, void*);
};

unsigned int
hash_function_servers(void *a);

unsigned int
hash_function_key(void *a);

hashtable_t *
ht_create(unsigned int hmax, unsigned int (*hash_function)(void*),
	int (*compare_function)(void*, void*));

void
ht_put(hashtable_t *ht, void *key, unsigned int key_size,
	void *value, unsigned int value_size);

void *
ht_get(hashtable_t *ht, void *key);

void
ht_remove_entry(hashtable_t *ht, void *key);

void
ht_free(hashtable_t *ht);

int
compare_function_strings(void *a, void *b);

#endif  // HASH_TABLE_H_
