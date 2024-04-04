/* Copyright 2021 Chioveanu Stelian-Ovidiu */
#include <stdlib.h>
#include <string.h>

#include "server.h"

/* Functia aloca memoria necesara unui server, il initializeaza
si ulterior il returneaza */
server_memory*
init_server_memory() {
	server_memory *server = calloc(1, sizeof(*server));
	DIE(!server, "calloc server failed");
	server->server_hashtable = ht_create(HMAX,
	hash_function_key, compare_function_strings);
	return server;
}

/* Functia adauga cheia si valoarea in serverul primit ca parametru */
void
server_store(server_memory* server, char* key, char* value) {
	ht_put(server->server_hashtable, key,
	strlen(key) + 1, value, strlen(value) + 1);
}

/* Functia sterge din server intrarea cheii primite ca parametru */
void
server_remove(server_memory* server, char* key) {
	ht_remove_entry(server->server_hashtable, key);
}

/* Functia returneaza valoarea corespunzatoarea cheii din server,
primite ca parametru*/
char*
server_retrieve(server_memory* server, char* key) {
	return ht_get(server->server_hashtable, key);
}

/* Functia elibereaza memoria utilizata de serverul primit ca parametru */
void
free_server_memory(server_memory* server) {
	ht_free(server->server_hashtable);
	free(server);
}
