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


//Create a new piece
// Piece create_piece(char id, int row, int col, int size, char orientation, int isPrimary) ;
// bool equal_piece(const Piece* a, const Piece* b);

//Check if piece can move

#endif 