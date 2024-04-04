/* Copyright 2021 Chioveanu Stelian-Ovidiu */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash_table.h"

/* Functii de hash (primite in schelet) */
unsigned int
hash_function_key(void *a) {
    unsigned char *puchar_a = (unsigned char *) a;
    unsigned int hash = 5381;
    int c;

    while ((c = *puchar_a++))
        hash = ((hash << 5u) + hash) + c;

    return hash;
}

unsigned int
hash_function_servers(void *a) {
    unsigned int uint_a = *((unsigned int *)a);

    uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
    uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
    uint_a = (uint_a >> 16u) ^ uint_a;
    return uint_a;
}

/* Functie de comparare a 2 stringuri */
int
compare_function_strings(void *a, void *b) {
	char *str_a = (char *)a;
	char *str_b = (char *)b;

	return strcmp(str_a, str_b);
}

/* Functia aloca memoria necesara unui hashtable si
initializeaza hmaxul, functia de comparare si functia de hash */
hashtable_t *
ht_create(unsigned int hmax, unsigned int (*hash_function)(void*),
		int (*compare_function)(void*, void*)) {
	hashtable_t *hashtable = calloc(1, sizeof(* hashtable));
	DIE(hashtable == NULL, "calloc hashtable failed");
	hashtable->buckets = malloc(hmax * sizeof(*(hashtable->buckets)));
	DIE(hashtable->buckets == NULL, "malloc buckets failed");
	hashtable->hmax = hmax;
	hashtable->compare_function = compare_function;
	hashtable->hash_function = hash_function;
	for (unsigned int i = 0; i < hmax; i++)
		hashtable->buckets[i] = ll_create(sizeof(struct info));
	return hashtable;
}

/* Functia adauga cheia alaturi de valoare in hashtable */
void
ht_put(hashtable_t *ht, void *key, unsigned int key_size,
	void *value, unsigned int value_size) {
	if (!ht){
		return;
	}
	unsigned int index = ht->hash_function(key) % ht->hmax;
	struct info info;
	info.value = malloc(value_size);
	DIE(info.value == NULL, "malloc info.value failed");
	info.key = malloc(key_size);
	DIE(info.key == NULL, "malloc info.key failed");
	memcpy(info.value, value, value_size);
	memcpy(info.key, key, key_size);
	ll_add_nth_node(ht->buckets[index], 0, &info);
	ht->size++;
}
/* Functia returneaza valoarea asociata cheii primite ca parametru,
iar daca cheia nu se afla in hashtable, va returna NULL */
void *
ht_get(hashtable_t *ht, void *key) {
	if (!ht){
		return NULL;
	}
	unsigned int index = ht->hash_function(key) % ht->hmax;
	ll_node_t *curr = ht->buckets[index]->head;
	while (curr) {
		struct info *info = (struct info*)curr->data;
		if (ht->compare_function(key, info->key) == 0){
			return info->value;
		}
		curr = curr->next;
	}
	return NULL;
}

/* Functia cauta in hashtable cheia primita ca parametru si
ii elimina intrarea, eliberand ulterior memoria alocata acesteia */
void
ht_remove_entry(hashtable_t *ht, void *key) {
	if (!ht){
		return;
	}
	unsigned int index = ht->hash_function(key) % ht->hmax;
	ll_node_t *curr = ht->buckets[index]->head;
	unsigned int index_list = 0;
	while (curr != NULL) {
		struct info *info = (struct info*)curr->data;
		ll_node_t *removed;
		if (ht->compare_function(key, info->key) == 0){
			free(info->value);
			free(info->key);
			removed = ll_remove_nth_node(ht->buckets[index], index_list);
			free(removed->data);
			free(removed);
			return;
		}
		curr = curr->next;
		index_list++;
	}
	return;
}

/* Functia elibereaza memoria utilizata de hashtable-ul ht */
void
ht_free(hashtable_t *ht) {
	for (unsigned int i = 0; i < ht->hmax; i++){
		linked_list_t *removed = ht->buckets[i];
		ll_node_t *curr = removed->head;
		while (curr) {
			struct info *info = (struct info*)curr->data;
			free(info->value);
			free(info->key);
			curr = curr->next;
		}
		ll_free(&removed);
	}
	free(ht->buckets);
	free(ht);
}
