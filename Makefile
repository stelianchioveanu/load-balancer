CC=gcc
CFLAGS=-std=c99 -Wall -Wextra
LOAD=load_balancer
SERVER=server
HASHRING=hashring
LINKEDLIST=linked_list
HASHTABLE=hash_table
SEARCHPOZ=search_pos
MOVEKEYS=move_keys

.PHONY: build clean

build: tema2

tema2: main.o $(LOAD).o $(SERVER).o $(HASHRING).o $(LINKEDLIST).o $(HASHTABLE).o $(SEARCHPOZ).o $(MOVEKEYS).o
	$(CC) $^ -o $@

main.o: main.c
	$(CC) $(CFLAGS) $^ -c

$(SERVER).o: $(SERVER).c $(SERVER).h
	$(CC) $(CFLAGS) $^ -c

$(LOAD).o: $(LOAD).c $(LOAD).h
	$(CC) $(CFLAGS) $^ -c

$(HASHRING).o: $(HASHRING).c $(HASHRING).h
	$(CC) $(CFLAGS) $^ -c

$(HASHTABLE).o: $(HASHTABLE).c $(HASHTABLE).h
	$(CC) $(CFLAGS) $^ -c

$(LINKEDLIST).o: $(LINKEDLIST).c $(LINKEDLIST).h
	$(CC) $(CFLAGS) $^ -c

$(SEARCHPOZ).o: $(SEARCHPOZ).c $(SEARCHPOZ).h
	$(CC) $(CFLAGS) $^ -c

$(MOVEKEYS).o: $(MOVEKEYS).c $(MOVEKEYS).h
	$(CC) $(CFLAGS) $^ -c

clean:
	rm -f *.o tema2 *.h.gch
