/* Copyright 2021 Chioveanu Stelian-Ovidiu */
#include <stdlib.h>
#include <string.h>

#include "hashring.h"

/* Functia adauga o replica in hashring la pozitia primita ca parametru
(shiftare) */
void
add_in_hashring(load_balancer *main, unsigned int pos, int label) {
    for (unsigned int i = main->nr_all_repl; i > pos; i--) {
        main->hash_ring[i] = main->hash_ring[i - 1];
    }
    main->hash_ring[pos] = label;
    main->nr_all_repl++;
}

/* Functia sterge din hashring replica de la pozitia primita ca parametru
(shiftare) */
void
remove_from_hashring(load_balancer *main, unsigned int pos) {
    for (unsigned int i = pos; i < main->nr_all_repl - 1; i++) {
        main->hash_ring[i] = main->hash_ring[i + 1];
    }
    main->hash_ring[main->nr_all_repl - 1] = 0;
    main->nr_all_repl--;
}
