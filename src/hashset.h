#ifndef HASHSET_H
#define HASHSET_H

#include "board.h"  // Pastikan bahwa Board didefinisikan sebelumnya

#define HASHSET_SIZE 1000  // Ukuran hash set

// Struktur untuk elemen dalam HashSet
typedef struct HashSetEntry {
    Board* state;  // Menyimpan pointer ke state Board
    struct HashSetEntry* next;
} HashSetEntry;

// Struktur HashSet
typedef struct HashSet {
    HashSetEntry* table[HASHSET_SIZE];  // Array untuk menyimpan entry
} HashSet;

HashSet* createHashSet();
void insertToHashSet(HashSet* set, Board* state);
bool containsInHashSet(HashSet* set, Board* state);
void freeHashSet(HashSet* set);

#endif
