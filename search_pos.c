/* Copyright 2021 Chioveanu Stelian-Ovidiu */
#include <stdlib.h>
#include <string.h>

#include "search_pos.h"

/* Functia foloseste o cautare binara speciala pentru a calcula pozitia in
hashring in functia de server_id (in cazul in care exista replici in hashring
cu aceeasi valoare hash). Functionalitatea este aceeasi cu cea de la functia
special_binary_search */
unsigned int
search_pos_in_hashring_by_server_id(load_balancer *main, unsigned int low,
    unsigned int high, int server_id) {
    unsigned int mid;
    while (low <= high) {
        mid = (low + high) / 2;
        if (server_id < (int)(main->hash_ring[low] % POW_TEN)) {
            return low;
        } else {
            if (server_id > (int)(main->hash_ring[high] % POW_TEN)) {
                return high + 1;
            } else {
                if (server_id > (int)(main->hash_ring[mid] % POW_TEN)) {
                    low = mid + 1;
                } else {
                    if (server_id < (int)(main->hash_ring[mid] % POW_TEN)){
                        high = mid - 1;
                    } else {
                        return mid;
                    }
                }
            }
        }
    }
    return 0;
}

/* Functia foloseste o cautare binara speciala pentru a calcula pozitia in
hashring in functia de hashul primit ca parametru (daca server_id este egal
cu -1 hashul reprezinta hashul unei chei, iar in caz contrar hashul unei replici) */
unsigned int
special_binary_search(load_balancer *main, int server_id, unsigned int hash) {
    unsigned int low = 0, high = main->nr_all_repl - 1, mid;
    while (low <= high){
        mid = (low + high) / 2;
        /* Daca hashul este mai mic decat hashul replicii de la pozitia low,
        returnam low (pozitia actuala) sau serverul corespunzator lui low */ 
        if (hash < hash_function_servers(&main->hash_ring[low])){
            if (server_id == -1) {
                return main->hash_ring[low] % POW_TEN;
            } else {
                return low;
            }
        } else {
            /* Daca hashul este mai mare decat hashul replicii de la pozitia high,
            returnam high + 1 (pozitia urmatoare) sau serverul corespunzator lui
            high + 1 */ 
            if (hash > hash_function_servers(&main->hash_ring[high])) {
                if (server_id == -1) {
                    return main->hash_ring[high + 1] % POW_TEN;
                } else {
                    return high + 1;
                }
            /* Impartim intervalul in 2 si gasim in care din ele se poate regasi hashul
            Acest else este corespunzator cazului in care hashul primit ca parametru este
            regasit in hashring de una sau mai multe ori (cazul in care cautam pozitia in
            in asa fel incat sa fie ordonate dupa server_id) */
            } else {
                if (hash > hash_function_servers(&main->hash_ring[mid])) {
                    low = mid + 1;
                } else {
                    if (hash <
                        hash_function_servers(&main->hash_ring[mid])){
                        high = mid - 1;
                    } else {
                        int new_low = mid, ok = 1, new_high = mid;
                        /* Parcurgem inapoi intervalul pana gasim prima replica cu hashul
                        egal cu hasul primit ca parametru */
                        while (ok){
                            if (new_low != 0 && hash ==
                                hash_function_servers(
                                &main->hash_ring[new_low - 1])){
                                new_low--;
                            } else {
                                ok = 0;
                            }
                        }
                        /* Daca hasul primit ca parametru este corespunzator unei chei
                        returnam serverul corespunzator primei replici cu hasul egal cu hashul
                        primit ca parametru */
                        if (server_id == -1) {
                            return main->hash_ring[new_low] % POW_TEN;
                        } else {
                        ok = 1;
                        /* Parcurgem inainte intervalul pana gasim ultima replica cu hashul
                        egal cu hasul primit ca parametru */
                        while (ok){
                            if (new_high != (int)main->nr_all_repl - 1 &&
                                hash == hash_function_servers(
                                &main->hash_ring[new_high + 1])){
                                new_high++;
                            } else {
                                ok = 0;
                            }
                        }
                        return search_pos_in_hashring_by_server_id(main,
                            new_low, new_high, server_id);
                        }
                    }
                }
            }
        }
    }
    return 0;
}

/* Functia returneaza pozitia la care putem adauga o replica in hashring */
unsigned int
search_pos_in_hashring(load_balancer *main, int label) {
    unsigned int hash = hash_function_servers(&label);
    unsigned int server_id = label % POW_TEN;
    /* Daca nu exista replici sau hashul replicii e mai mic decat
    hashul primei replici din hashring returnam 0 */
    if (main->nr_all_repl == 0 || hash <
        hash_function_servers(&main->hash_ring[0])) {
        return 0;
    }
    /* Daca hashul replicii e mai mare decat hashul ultimei replici din hashring
    intoartem pozitia de final */
    if (hash > hash_function_servers(&main->hash_ring[main->nr_all_repl - 1])) {
        return main->nr_all_repl;
    }
    return special_binary_search(main, server_id, hash);
}

/* Functia returneaza pozitia corepunzatoare replicii pe al carui server putem
introduce cheia si valoarea */
unsigned int
search_server_by_key(load_balancer *main, unsigned int hash) {
    /* Daca hashul cheii e mai mic decat hashul primei replici din hashring
    intoartem serverul corespunzator primei replici */
    if (hash < hash_function_servers(&main->hash_ring[0])) {
        return main->hash_ring[0] % POW_TEN;
    }
    /* Daca hashul cheii e mai mic decat hashul ultimei replici din hashring
    intoartem serverul corespunzator primei replici */
    if (hash > hash_function_servers(&main->hash_ring[main->nr_all_repl - 1])) {
        return main->hash_ring[0] % POW_TEN;
    }
    return special_binary_search(main, -1, hash);
}
