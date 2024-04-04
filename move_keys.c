/* Copyright 2021 Chioveanu Stelian-Ovidiu */
#include <stdlib.h>
#include <string.h>

#include "move_keys.h"

/* Functia redirectioneaza cheile si valorile din serverul 2 (server_id2) in
serverul 1 (server_id1)*/
void
search_key_and_move(load_balancer *main, unsigned int hash1, unsigned hash2,
    int val, int server_id1, int server_id2) {
    ll_node_t *curr;
    // Parcurgem dictionarul serverul 2
    for (int i = 0; i < HMAX; i++) {
        curr = main->servers[server_id2]->server_hashtable->buckets[i]->head;
        while (curr != NULL) {
            struct info *info = (struct info*)curr->data;
            int ok = 1;
            /* Pentru val egal cu 0 avem conditia de a muta cheile pe serverul
            replicii de pe prima pozitie */
            if (!val) {
                if (hash_function_key(info->key) <= hash1 ||
                    hash2 < hash_function_key(info->key)) {
                    server_store(main->servers[server_id1],
                                info->key, info->value);
                    ok = 0;
                }
            } else {
                if (hash1 >= hash_function_key(info->key) &&
                    hash2 < hash_function_key(info->key)){
                    server_store(main->servers[server_id1],
                                info->key, info->value);
                    ok = 0;
                }
            }
            curr = curr->next;
            if (ok == 0) {
                // Daca am mutat cheia o eliminam din server
                server_remove(main->servers[server_id2], info->key);
            }
        }
    }
}

/* Functia muta cheile si valorile din serverele actuale in cel nou adaugat
dupa pozitia cheii primite ca parametru*/
void
move_keys_add(load_balancer *main, unsigned int pos) {
    int server_id1 = main->hash_ring[pos] % POW_TEN;
    /* hash1 = hashul replicii de pe al carui server mutam cheile,
    hash2 = hashul replicii pe al carui server mutam cheile, */
    unsigned int hash1 = hash_function_servers(&main->hash_ring[pos]), hash2;
    int server_id2 = main->hash_ring[(pos + 1) % main->nr_all_repl] % POW_TEN;
    if (pos == 0){
        hash2 = hash_function_servers(&main->hash_ring[main->nr_all_repl - 1]);
        // Pentru pozitia egala cu 0 al patrulea argument este 0
        search_key_and_move(main, hash1, hash2, 0, server_id1, server_id2);
    } else {
        hash2 = hash_function_servers(&main->hash_ring[pos - 1]);
        // Pentru pozitia diferita de 0 al patrulea argument este 1
        search_key_and_move(main, hash1, hash2, 1, server_id1, server_id2);
    }
}

/* Functia muta cheile si valorile din serverul eliminat in cele ramase dupa
pozitia cheii primite ca parametru*/
void
move_keys_remove(load_balancer *main, unsigned int pos) {
    int server_id1 = main->hash_ring[pos] % POW_TEN;
    /* hash1 = hashul replicii de pe al carui server mutam cheile,
    hash2 = hashul replicii pe al carui server mutam cheile, */
    unsigned int hash1 = hash_function_servers(&main->hash_ring[pos]), hash2;
    int server_id2 = main->hash_ring[(pos + 1) % main->nr_all_repl] % POW_TEN;
    if (pos == 0){
        hash2 = hash_function_servers(&main->hash_ring[main->nr_all_repl - 1]);
        // Pentru pozitia egala cu 0 al patrulea argument este 0
        search_key_and_move(main, hash1, hash2, 0, server_id2, server_id1);
    } else {
        hash2 = hash_function_servers(&main->hash_ring[pos - 1]);
        // Pentru pozitia diferita de 0 al patrulea argument este 1
        search_key_and_move(main, hash1, hash2, 1, server_id2, server_id1);
    }
}
