// pathfinder.h
#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "board.h"
#include "piece.h"
#include "utils.h"
#include "priorityqueue.h"
#include "hashset.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>


typedef struct Node {
    Board* state;
    struct Node* parent;
    Move move; // Move that led to this state
    int g; // Cost from start to this node
    int h; // Heuristic cost to goal
    int f; // Total cost (g + h)
} Node;

Node* createNode(Board* state, int g, int h,  Node* parent,Move* move);



bool isStateExplored(HashSet* exploredStates, Board* state);;

void reconstructPath(Board* startNode, Node* goalNode);

int heuristic1(const Board *board);// Manhattan Distance to Exit

Board** generateNextStates(const Board *board, int *nextStatesCount, Move* moves);

Node** generateChildNodes(Node* parent, int* count, int heuristicChoice);

int compareGBFS(const Node* a, const Node* b);

int compareUCS(const Node* a, const Node* b);

int compareAStar(const Node* a, const Node* b);
Node* genericSearch(Board initialBoard, int heuristicChoice, int (*compare)(const Node*, const Node*)) ;

Node* greedyBestFirstSearch(Board initialBoard, int heuristicChoice);

Node* uniformCostSearch(Board initialBoard);

Node* aStarSearch(Board initialBoard, int heuristicChoice);


// int countBlockingPieces(Board* board);

// Board* solveUCS (Board* start);





#endif