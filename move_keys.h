/* Copyright 2021 Chioveanu Stelian-Ovidiu */
#ifndef MOVE_KEYS_H_
#define MOVE_KEYS_H_

#include "load_balancer.h"

void
move_keys_add(load_balancer *main, unsigned int pos);

void
move_keys_remove(load_balancer *main, unsigned int pos);

void
search_key_and_move(load_balancer *main, unsigned int hash1,
    unsigned hash2, int val, int server_id1, int server_id2);

#endif  // MOVE_KEYS_H_
