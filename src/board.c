#include "board.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define RESET_COLOR "\x1b[0m"

int totalCheck = 0;

void initBoard(Board *board) {
    board->rows = 0;
    board->cols = 0;
    board->grid = NULL;
    board->numPieces = 0;
    board->pieces = NULL;
    board->primaryPieceID = '\0';
    board->primaryIndex = -1;
    board->exit_row = -1;
    board->exit_col = -1;
    board->solution = NULL;
    board->solutionLength = 0;
}
const char* getPieceColor(char pieceId) {
    // Map each piece ID to a unique color code (256-color range)
    switch (pieceId) {
        case 'A': return "\x1b[38;5;1m";  // Red
        case 'B': return "\x1b[38;5;2m";  // Green
        case 'C': return "\x1b[38;5;3m";  // Yellow
        case 'D': return "\x1b[38;5;4m";  // Blue
        case 'E': return "\x1b[38;5;5m";  // Magenta
        case 'F': return "\x1b[38;5;6m";  // Cyan
        case 'G': return "\x1b[38;5;7m";  // White
        case 'H': return "\x1b[38;5;8m";  // Gray
        case 'I': return "\x1b[38;5;9m";  // Bright Red
        case 'J': return "\x1b[38;5;10m"; // Bright Green
        case 'K': return "\x1b[38;5;11m"; // Bright Yellow
        case 'L': return "\x1b[38;5;12m"; // Bright Blue
        case 'M': return "\x1b[38;5;13m"; // Bright Magenta
        case 'N': return "\x1b[38;5;14m"; // Bright Cyan
        case 'O': return "\x1b[38;5;15m"; // Bright White
        case 'P': return "\x1b[38;5;16m"; // Dark Red
        case 'Q': return "\x1b[38;5;17m"; // Dark Green
        case 'R': return "\x1b[38;5;18m"; // Dark Yellow
        case 'S': return "\x1b[38;5;19m"; // Dark Blue
        case 'T': return "\x1b[38;5;20m"; // Dark Magenta
        case 'U': return "\x1b[38;5;21m"; // Dark Cyan
        case 'V': return "\x1b[38;5;22m"; // Dark Gray
        case 'W': return "\x1b[38;5;23m"; // Light Red
        case 'X': return "\x1b[38;5;24m"; // Light Green
        case 'Y': return "\x1b[38;5;25m"; // Light Yellow
        case 'Z': return "\x1b[38;5;26m"; // Light Blue
        default: return "\x1b[38;5;7m"; // Default color (White)
    }
}

void printBoard(const Board *board) {
    if (!board || !board->grid) return; // Safety check

    if(board->exit_row == -1) {
        for (int j = 0; j < board->cols; j++) {
            if (board->exit_col != j){
                printf("  ");
            }
            if (board->exit_col ==j){
                printf("%sK%s", "\x1b[38;5;11m", RESET_COLOR);
            }
        }
        printf("\n");
    }
    
    //jika exit di kirinya
    if(board->exit_col== -1){
        for (int i = 0; i < board->rows; i++) {
            if(board->exit_row == i && board->exit_col ==-1) {
                    printf("%sK%s ", "\x1b[38;5;11m", RESET_COLOR);
            }
            if (board->exit_row != i){
                printf("  ");
            }
            for (int j = 0; j < board->cols; j++) {
                char pieceId = board->grid[i][j];
                const char* color = getPieceColor(pieceId);
                printf("%s%c%s ", color, pieceId, RESET_COLOR);
            }
            
            printf("\n");
        }
    }
    //jika exit di kanannya
    if (board->exit_col !=-1){
        for (int i = 0; i < board->rows; i++) {
        
            for (int j = 0; j < board->cols; j++) {
                char pieceId = board->grid[i][j];
                const char* color = getPieceColor(pieceId);
                printf("%s%c%s ", color, pieceId, RESET_COLOR);
                if(board->exit_row == i && (j == board->cols -1) &&board->exit_col !=-1) {
                    printf("%sK%s", "\x1b[38;5;11m", RESET_COLOR);
                }
            }
            
            printf("\n");
        }
    }
    
   
}
// void printBoard(const Board *board) {
//   if (!board || !board->grid) return; // Safety check

//   for (int i = 0; i < board->rows; i++) {
//     for (int j = 0; j < board->cols; j++) {
//       printf("%c ", board->grid[i][j]);
//     }
//     printf("\n");
//   }
// }

int findPieceIndex(const Board *board, char pieceId) {
  for (int i = 0; i < board->numPieces; i++) {
    if (board->pieces[i].id == pieceId) {
      return i;
    }
  }
  return -1; // Not found
}

bool canMove(const Board *board, const Piece *piece, char direction, int steps) {
    if (!board || !piece) return false; // Safety check

    int newRow = piece->row;
    int newCol = piece->col;

    totalCheck++;
    // Tentukan posisi baru berdasarkan orientasi dan arah
    if (piece->orientation == 'H') {
        if (direction == 'L') newCol -= steps;
        else if (direction == 'R') newCol += steps;
        else return false; // Arah tidak valid untuk horizontal
    } else if (piece->orientation == 'V') {
        if (direction == 'U') newRow -= steps;
        else if (direction == 'D') newRow += steps;
        else return false; // Arah tidak valid untuk vertikal
    } else {
        return false; // Orientasi tidak valid
    }

    // Periksa jika seluruh bagian piece masih dalam batas papan
    if (piece->orientation == 'H') {
        // Memastikan seluruh panjang piece horizontal masih dalam batas
        if (newCol < 0 || newCol + piece->size > board->cols) {
            return false; // Pergerakan keluar batas papan
        }
    } else if (piece->orientation == 'V') {
        // Memastikan seluruh panjang piece vertikal masih dalam batas
        if (newRow < 0 || newRow + piece->size > board->rows) {
            return false; // Pergerakan keluar batas papan
        }
    }

    // Cek tabrakan dengan piece lain
    for (int i = 0; i < board->numPieces; i++) {
        if (board->pieces[i].id == piece->id) continue; // Skip piece itu sendiri

        int otherRow = board->pieces[i].row;
        int otherCol = board->pieces[i].col;
        int otherSize = board->pieces[i].size;
        char otherOrientation = board->pieces[i].orientation;

        // Periksa semua cell yang akan ditempati oleh piece yang sedang bergerak
        for (int s = 0; s < piece->size; s++) {
            int checkRow = piece->row;
            int checkCol = piece->col;
            if (piece->orientation == 'H') checkCol = newCol + s; // Horizontal, periksa semua kolom
            else checkRow = newRow + s; // Vertical, periksa semua baris

            // Periksa tabrakan dengan piece lain
            for (int os = 0; os < otherSize; os++) {
                int otherCheckRow = otherRow;
                int otherCheckCol = otherCol;

                if (otherOrientation == 'H') otherCheckCol = otherCol + os; // Horizontal
                else otherCheckRow = otherRow + os; // Vertical

                // Jika posisi baru piece bertabrakan dengan piece lain, return false
                if (checkRow == otherCheckRow && checkCol == otherCheckCol) {
                    return false; // Collision
                }
            }
        }
    }

    return true; // Valid move
}


void movePiece(Board *board, char pieceId, char direction, int steps) {
    if (!board) return;

    int pieceIndex = findPieceIndex(board, pieceId);
    if (pieceIndex == -1) return;

    Piece *pieceToMove = &board->pieces[pieceIndex];

    if (canMove(board, pieceToMove, direction, steps)) {
        int oldRow = pieceToMove->row;
        int oldCol = pieceToMove->col;

        // Clear old position
        for (int i = 0; i < pieceToMove->size; i++) {
            if (pieceToMove->orientation == 'H') board->grid[oldRow][oldCol + i] = '.';
            else board->grid[oldRow + i][oldCol] = '.';
        }

        // Update position
        if (pieceToMove->orientation == 'H') {
            if (direction == 'L') pieceToMove->col -= steps;
            else if (direction == 'R') pieceToMove->col += steps;
        } else {
            if (direction == 'U') pieceToMove->row -= steps;
            else if (direction == 'D') pieceToMove->row += steps;
        }

        // Draw new position
        for (int i = 0; i < pieceToMove->size; i++) {
            if (pieceToMove->orientation == 'H') board->grid[pieceToMove->row][pieceToMove->col + i] = pieceId;
            else board->grid[pieceToMove->row + i][pieceToMove->col] = pieceId;
        }
    }
}

bool isSolved(const Board *board) {
    if (!board) return false;

    int primaryPieceIndex = findPieceIndex(board, board->primaryPieceID);
    if (primaryPieceIndex == -1) return false;

    Piece p = board->pieces[primaryPieceIndex];

    // Horizontal piece
    if (p.orientation == 'H') {
        // Check right
        for (int col = p.col + p.size; col <= board->cols; col++) {
            if (col == board->exit_col && p.row == board->exit_row) return true;
            if (col >= board->cols || board->grid[p.row][col] != '.') break;
        }
        // Check left
        for (int col = p.col - 1; col >= -1; col--) {
            if (col == board->exit_col && p.row == board->exit_row) return true;
            if (col < 0 || board->grid[p.row][col] != '.') break;
        }
    }

    // Vertical piece
    if (p.orientation == 'V') {
        // Check down
        for (int row = p.row + p.size; row <= board->rows; row++) {
            if (row == board->exit_row && p.col == board->exit_col) return true;
            if (row >= board->rows || board->grid[row][p.col] != '.') break;
        }
        // Check up
        for (int row = p.row - 1; row >= -1; row--) {
            if (row == board->exit_row && p.col == board->exit_col) return true;
            if (row < 0 || board->grid[row][p.col] != '.') break;
        }
    }

    return false;
}

void freeBoard(Board *board) {
    if (board) {
        // Free the grid
        if (board->grid) {
            for (int i = 0; i < board->rows; i++) {
                free(board->grid[i]);
            }
            free(board->grid);
        }
        // Free the pieces array
        if (board->pieces) {
            free(board->pieces);
        }
        // Free the solution (if it exists)
        if (board->solution) {
            free(board->solution);
        }
        free(board); // Free the Board struct itself
    }
}


Board *copyBoard(const Board *src) {
    if (!src) return NULL;

    Board *dest = (Board *)malloc(sizeof(Board));
    if (!dest) return NULL;

    dest->rows = src->rows;
    dest->cols = src->cols;

    // Allocate and copy grid
    dest->grid = (char **)malloc(src->rows * sizeof(char *));
    if (!dest->grid) {
        free(dest);
        return NULL;
    }
    for (int i = 0; i < src->rows; i++) {
        dest->grid[i] = (char *)malloc(src->cols * sizeof(char));
        if (!dest->grid[i]) {
            for (int j = 0; j < i; j++) {
                free(dest->grid[j]);
            }
            free(dest->grid);
            free(dest);
            return NULL;
        }
        memcpy(dest->grid[i], src->grid[i], src->cols * sizeof(char));
    }

    dest->numPieces = src->numPieces;

    // Allocate and copy pieces
    dest->pieces = (Piece *)malloc(src->numPieces * sizeof(Piece));
    if (!dest->pieces) {
        for (int i = 0; i < src->rows; i++) {
            free(dest->grid[i]);
        }
        free(dest->grid);
        free(dest);
        return NULL;
    }
    memcpy(dest->pieces, src->pieces, src->numPieces * sizeof(Piece));

    dest->primaryPieceID = src->primaryPieceID;
    dest->primaryIndex = src->primaryIndex;
    dest->exit_row = src->exit_row;
    dest->exit_col = src->exit_col;

    dest->solutionLength = src->solutionLength;
    if (src->solutionLength > 0) {
        dest->solution = (Move *)malloc(src->solutionLength * sizeof(Move));
        if (!dest->solution) {
            for (int i = 0; i < src->rows; i++) {
                free(dest->grid[i]);
            }
            free(dest->grid);
            free(dest->pieces);
            free(dest);
            return NULL;
        }
        memcpy(dest->solution, src->solution, src->solutionLength * sizeof(Move));
    } else {
        dest->solution = NULL;
    }

    return dest;
}

bool compareBoards(const Board* a, const Board* b) {
    if (a->rows != b->rows || a->cols != b->cols || a->numPieces != b->numPieces ||
        a->exit_row != b->exit_row || a->exit_col != b->exit_col ||
        a->primaryPieceID != b->primaryPieceID || a->primaryIndex != b->primaryIndex) {
        return false;
    }
    for (int i = 0; i < a->rows; i++) {
        if (memcmp(a->grid[i], b->grid[i], a->cols) != 0) {
            return false;
        }
    }
    for (int i = 0; i < a->numPieces; i++) {
        if (a->pieces[i].id != b->pieces[i].id || a->pieces[i].row != b->pieces[i].row ||
            a->pieces[i].col != b->pieces[i].col || a->pieces[i].size != b->pieces[i].size ||
            a->pieces[i].orientation != b->pieces[i].orientation || a->pieces[i].isPrimary != b->pieces[i].isPrimary) {
            return false;
        }
    }
    return true;
}