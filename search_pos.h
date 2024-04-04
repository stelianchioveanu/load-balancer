/* Copyright 2021 Chioveanu Stelian-Ovidiu */
#ifndef SEARCH_POS_H_
#define SEARCH_POS_H_

#include "load_balancer.h"

unsigned int
search_pos_in_hashring(load_balancer *main, int label);

unsigned int
search_pos_in_hashring_by_server_id(load_balancer *main,
    unsigned int low, unsigned int high, int server_id);

unsigned int
search_server_by_key(load_balancer *main, unsigned int hash);

unsigned int
special_binary_search(load_balancer *main, int server_id, unsigned int hash);

#endif  // SEARCH_POS_H_
