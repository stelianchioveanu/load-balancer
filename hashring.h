/* Copyright 2021 Chioveanu Stelian-Ovidiu */
#ifndef HASHRING_H_
#define HASHRING_H_

#include "load_balancer.h"

void
add_in_hashring(load_balancer *main, unsigned int pos, int label);

void
remove_from_hashring(load_balancer *main, unsigned int pos);

#endif  // HASHRING_H_
