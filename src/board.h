#ifndef BOARD_H
#define BOARD_H

#include "piece.h"


typedef struct {
    char pieceId;
    char direction; // 'U', 'D', 'L', 'R'
    int steps;
} Move;

typedef struct {
    int rows;
    int cols;
    char** grid;
    int numPieces;
    Piece *pieces;
    char primaryPieceID;
    int primaryIndex; // Index of the primary piece in the pieces array
    int exit_row;
    int exit_col;
    Move *solution; // List of moves to reach the solution
    int solutionLength;
} Board;

extern int totalCheck;
void initBoard(Board *board);
void printBoard(const Board *board);

bool compareBoards(const Board *a, const Board *b);

int findPieceIndex(const Board *board, char pieceId);
bool canMove(const Board *board, const Piece *piece, char direction, int steps);
void movePiece(Board *board, char pieceId, char direction, int steps);



bool isSolved(const Board* board);
void freeBoard(Board *board); //free board and its pieces

Board* copyBoard(const Board* src);


#endif