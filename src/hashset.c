#include "hashset.h"
#include <stdlib.h>
#include <string.h>

// Fungsi hash untuk Board
int hash(Board* state) {
    int hashValue = 0;
    for (int i = 0; i < state->rows; i++) {
        for (int j = 0; j < state->cols; j++) {
            hashValue = (hashValue * 31 + state->grid[i][j]) % HASHSET_SIZE;
        }
    }
    return hashValue;
}

// Membuat HashSet baru
HashSet* createHashSet() {
    HashSet* set = (HashSet*)malloc(sizeof(HashSet));
    for (int i = 0; i < HASHSET_SIZE; i++) {
        set->table[i] = NULL;
    }
    return set;
}

// Menambahkan state ke HashSet
void insertToHashSet(HashSet* set, Board* state) {
    int index = hash(state);
    HashSetEntry* newEntry = (HashSetEntry*)malloc(sizeof(HashSetEntry));
    newEntry->state = state;
    newEntry->next = set->table[index];
    set->table[index] = newEntry;
}

// Memeriksa apakah state sudah ada dalam HashSet
bool containsInHashSet(HashSet* set, Board* state) {
    int index = hash(state);
    HashSetEntry* entry = set->table[index];
    while (entry != NULL) {
        if (compareBoards(entry->state, state)) {
            return true;
        }
        entry = entry->next;
    }
    return false;
}

// Membebaskan memori untuk HashSet
void freeHashSet(HashSet* set) {
    for (int i = 0; i < HASHSET_SIZE; i++) {
        HashSetEntry* entry = set->table[i];
        while (entry != NULL) {
            HashSetEntry* temp = entry;
            entry = entry->next;
            free(temp);
        }
    }
    free(set);
}
