/* Copyright 2021 Chioveanu Stelian-Ovidiu */
#include <stdlib.h>
#include <string.h>

#include "load_balancer.h"
#include "hashring.h"
#include "search_pos.h"
#include "move_keys.h"

/* Functia aloca si initializeaza un load balancer */
load_balancer*
init_load_balancer() {
	load_balancer *main_load = calloc(1, sizeof(*main_load));
    DIE(!main_load, "calloc load_balancer failed");
    main_load->hash_ring = calloc(NUMBER_OF_REPLICAS *
    (MAX_NUMBER_SERVERS + 1),
    sizeof(*main_load->hash_ring));
    DIE(!main_load->hash_ring, "calloc hash_ring failed");
    main_load->servers = calloc((MAX_NUMBER_SERVERS + 1),
    sizeof(*main_load->servers));
    DIE(!main_load->servers, "calloc servers failed");
    return main_load;
}

/* Functia cauta serverul corespunzator cheii si o adauga in el */
void
loader_store(load_balancer* main, char* key, char* value, int* server_id) {
    *server_id = search_server_by_key(main, hash_function_key(key));
    server_store(main->servers[*server_id], key, value);
}

/* Functia intoarce valoarea corespunzatoare cheii pe care o gaseste
in servere */
char*
loader_retrieve(load_balancer* main, char* key, int* server_id) {
	*server_id = search_server_by_key(main, hash_function_key(key));
    return server_retrieve(main->servers[*server_id], key);
}

/* Functia adauga un server in load balancer + (ii adauga replicile in
hashring) */
void
loader_add_server(load_balancer* main, int server_id) {
    int label;
    unsigned int pos;
    main->servers[server_id] = init_server_memory();
    // Construim replicile
    for (int i = 0; i < NUMBER_OF_REPLICAS; i++) {
        label = i * POW_TEN + server_id;
        // Cautam pozitia replicii in hashring
        pos = search_pos_in_hashring(main, label);
        // Adaugam replica la pozitia gasita
        add_in_hashring(main, pos, label);
        if (main->nr_all_repl > NUMBER_OF_REPLICAS)
            // Mutam cheile din serverele actuale in cel nou adaugat
            move_keys_add(main, pos);
    }
}

/* Functia sterge un server din load balancer + (ii sterge replicile in
hashring) */
void
loader_remove_server(load_balancer* main, int server_id) {
    int label;
    unsigned int pos;
    // Construim replicile
    for (int i = 0; i < NUMBER_OF_REPLICAS; i++) {
        label = i * POW_TEN + server_id;
        // Cautam pozitia replicii in hashring
        pos = search_pos_in_hashring(main, label);
        if (main->nr_all_repl > NUMBER_OF_REPLICAS) {
            // Mutam cheile din serverul ce urmeaza a fi sters in cele ramase
            move_keys_remove(main, pos);
        }
        // Stergem replica de la pozitia gasita
        remove_from_hashring(main, pos);
    }
    // Eliberaam memoria serverului sters
	free_server_memory(main->servers[server_id]);
    main->servers[server_id] = NULL;
}

/* Functia elibereaza memoria alocata de load balancer */
void
free_load_balancer(load_balancer* main) {
    for (int i = 0; i <= MAX_NUMBER_SERVERS; i++){
        if (main->servers[i] != NULL)
            free_server_memory(main->servers[i]);
    }
    free(main->servers);
    free(main->hash_ring);
    free(main);
}
