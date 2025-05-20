//main.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"
#include "utils.h"
#include "piece.h"
#include "priorityqueue.h"
#include "pathfinder.h"
#include "hashset.h"

#include <stdbool.h>
#include "time.h"

#include "piece.c"
#include "board.c"
#include "pathfinder.c"
#include "priorityqueue.c"
#include "utils.c"
#include "hashset.c"

int main() {
    char filename[256];
    printf("Enter the input filename: ");
    scanf("%s", &filename);

    Board initialBoard;

    initBoard(&initialBoard);
    
    // Board *board = loadBoardFromFile(filename);
    
    // if (!board) {
    //     printf("Failed to load board\n");
    //     return 1;
    // }

    if (!loadAndInitializeBoard(filename, &initialBoard)) {
        printf("Failed to load and initialize board\n");
        return 1;
    }
    
    printf("Papan awal:\n");
    printBoard(&initialBoard);
    
    int algorithmChoice,heuristicChoice;

    printf("Choose an algorithm:\n");
    printf("1. Greedy Best-First Search\n");
    printf("2. Uniform Cost Search\n");
    printf("3. A* Search\n");
    scanf("%d", &algorithmChoice);

    if (algorithmChoice == 1 || algorithmChoice == 3) {
        printf("Choose a heuristic:\n");
        printf("1. Manhattan Distance to Exit\n");
        printf("2. Number of Blocking Pieces\n");
        scanf("%d", &heuristicChoice);
    }

    clock_t t1, t2;
    t1 = clock();
    printf("==========================\n");
    //====algorithm
    Node* goalNode = NULL;

    
    switch (algorithmChoice) {
        case 1:
            printf("Running Greedy Best-First Search with heuristic %d...\n", heuristicChoice);
            goalNode = greedyBestFirstSearch(initialBoard, heuristicChoice);
            break;
        case 2:
            printf("Running Uniform Cost Search...\n");
            goalNode = uniformCostSearch(initialBoard);
            break;
        case 3:
            printf("Running A* Search with heuristic %d...\n", heuristicChoice);
            goalNode = aStarSearch(initialBoard, heuristicChoice);
            break;
        default:
            printf("Invalid algorithm choice.\n");
            
            return 1;
    }

    //=====
    
    t2 = clock();

    double waktu_ms = (double)(t2 - t1) * 1000.0 / CLOCKS_PER_SEC;
    

    if (goalNode != NULL) {
        printf("Solusi Ditemukan:\n\n");
        reconstructPath(&initialBoard, goalNode);
        
        // Count the number of moves in the solution`
        int moveCount = 0;
        Node* current = goalNode;
        while (current->parent != NULL) {
            moveCount++;
            current = current->parent;
        }
        
        printf("Jumlah gerakan yang diperiksa %d\n", totalCheck);
        printf("Banyaknya gerakan yang dibutuhkan: %d\n", moveCount);
        
        // Free memory for the solution path
        current = goalNode;
        while (current != NULL) {
            Node* temp = current;
            current = current->parent;
            if (temp->state) {
                freeBoard(temp->state);
            }
            free(temp);
        }
        printf("\n");
        printf("Waktu Eksekusi: %.2f ms\n", waktu_ms);

    
    } else {
        printf("Solusi tidak ditemukan.\n");
        printf("Banyaknya gerakan: 0\n");
    }

    freeBoard(&initialBoard);
    return 0;

    // printBoard(board);
    // printf("piece count: %d\n", board->numPieces);
    // printf("Primary piece ID: %c\n", board->primaryPieceID);
    // printf("Primary piece index: %d\n", board->primaryIndex);
    // printf("Board dimensions: %d x %d\n", board->rows, board->cols);
    // printf("Pieces on the board:\n");
    // for (int i = 0; i < board->numPieces; i++) {
    //     Piece *piece = &board->pieces[i];
    //     printf("Piece ID: %c, Row: %d, Col: %d, Size: %d, Orientation: %c\n",
    //            piece->id, piece->row, piece->col, piece->size, piece->orientation);
    // }
    // printf("Exit position: (%d, %d)\n", board->exit_row, board->exit_col);

    // Example of moving a piece
    // movePiece(board, 'P', 'D', 1); // Move piece 'P' down by 1 step

    // if (isSolved(board)) {
    //     printf("Puzzle solved!\n");
    // } else {
    //     printf("Puzzle not solved yet.\n");
    // }

    // freeBoard(board);
    // return 0;
}