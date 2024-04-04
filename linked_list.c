/* Copyright 2021 Chioveanu Stelian-Ovidiu */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linked_list.h"

/* Functia aloca memorie pentru o lista simplu inlantuita */
linked_list_t *
ll_create(unsigned int data_size) {
    linked_list_t* ll = calloc(1, sizeof(*ll));
    DIE(ll == NULL, "calloc ll failed");
    ll->data_size = data_size;
    return ll;
}

/* Functia adauga un nod in lista simplu inlantuita pe pozitia n */
void
ll_add_nth_node(linked_list_t* list, unsigned int n, const void* new_data) {
    if (list == NULL) {
        return;
    }
    ll_node_t *prev = NULL, *curr = list->head;
    ll_node_t* new_node = malloc(sizeof(*new_node));
    DIE(new_node == NULL, "new_node malloc");
    new_node->data = malloc(list->data_size);
    DIE(new_node->data == NULL, "new_node->data malloc");
    memcpy(new_node->data, new_data, list->data_size);
    if (n > list->size) {
        n = list->size;
    }
    while (n > 0) {
        prev = curr;
        curr = curr->next;
        n--;
    }
    new_node->next = curr;
    if (!prev) {
        list->head = new_node;
    } else {
        prev->next = new_node;
    }
    list->size++;
}

/* Functia elimina un nod din lista simplu inlantuita si ii returneaza
adresa pentru ca ulterior sa i se elibereze memoria */
ll_node_t *
ll_remove_nth_node(linked_list_t* list, unsigned int n) {
    unsigned int i;
    if (!list || !list->head){
        return NULL;
    }
    ll_node_t *prev = NULL, *curr = list->head;
    if (!n){
        list->head = list->head->next;
    } else {
        if (n > 0 && n < list->size - 1){
            for (i = 0; i < n; i++){
                prev = curr;
                curr = curr->next;
            }
            prev->next = curr->next;
        } else {
            while (curr->next) {
                prev = curr;
		        curr = curr->next;
	        }
            prev->next = NULL;
        }
    }
    list->size--;
    return curr;
}

/* Functia returneaza numarul de noduri din lista */
unsigned int
ll_get_size(linked_list_t* list) {
    if (!list) {
        return 0;
    } else {
        return list->size;
    }
}

/* Functia elibereaza memoria alocata pentru lista simplu inlantuita */
void
ll_free(linked_list_t** pp_list) {
    ll_node_t* curr;
    if (!pp_list || !*pp_list) {
        return;
    }
    while (ll_get_size(*pp_list) > 0) {
        curr = ll_remove_nth_node(*pp_list, 0);
        free(curr->data);
        free(curr);
    }
    free(*pp_list);
    *pp_list = NULL;
}
