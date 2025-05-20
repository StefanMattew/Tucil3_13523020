// piece.h
#ifndef PIECE_H
#define PIECE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    char id;
    int row;
    int col;
    int size;
    char orientation; // H: horizontal, V: vertical
    int isPrimary; // 1 if primary piece, 0 otherwise

} Piece;



#endif 