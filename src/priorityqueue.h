//priorityqueue.h
#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include "board.h"

typedef struct Node {
    Board* state;
    struct Node* parent;
    Move move; // Move that led to this state
    int g; // Cost from start to this node
    int h; // Heuristic cost to goal
    int f; // Total cost (g + h)
} Node;



typedef struct {
    Node** elements;
    int size;
    int capacity;
    int (*compare)(const Node*, const Node*);
} PriorityQueue;

// PriorityQueue* createPriorityQueue(int capacity, int (*compare)(const Node*, const Node*));

PriorityQueue* createPriorityQueue(int (*compare)(const Node*, const Node*));

void enqueue(PriorityQueue* pq, Node* node);

Node* dequeue(PriorityQueue* pq);

void freePriorityQueue(PriorityQueue* pq);

int isEmpty(PriorityQueue* pq);
#endif