#include "priorityqueue.h"

PriorityQueue* createPriorityQueue(int (*compare)(const Node*, const Node*)) {
    int capacity = 10; // Default initial capacity
    PriorityQueue* pq = (PriorityQueue*)malloc(sizeof(PriorityQueue));
    if (!pq) return NULL;
    pq->capacity = capacity;
    pq->size = 0;
    pq->elements = (Node**)malloc(capacity * sizeof(Node*));
    if (!pq->elements) {
        free(pq);
        return NULL;
    }
    pq->compare = compare;
    return pq;
}

void enqueue(PriorityQueue* pq, Node* node) {
    if (pq->size == pq->capacity) {
        pq->capacity *= 2;
        Node** temp = (Node**)realloc(pq->elements, pq->capacity * sizeof(Node*));
        if (!temp) {
            fprintf(stderr, "Reallocation failed for priority queue\n");
            return;
        }
        pq->elements = temp;
    }
    pq->elements[pq->size++] = node;
    // Heapify up (sift up)
    int i = pq->size - 1;
    while (i > 0 && pq->compare(pq->elements[(i - 1) / 2], pq->elements[i]) > 0) {
        Node* temp = pq->elements[i];
        pq->elements[i] = pq->elements[(i - 1) / 2];
        pq->elements[(i - 1) / 2] = temp;
        i = (i - 1) / 2;
    }
}

Node* dequeue(PriorityQueue* pq) {
    if (pq->size == 0) return NULL;
    Node* min = pq->elements[0];
    pq->elements[0] = pq->elements[--pq->size];
    // Heapify down (sift down)
    int i = 0;
    while (true) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = i;
        if (left < pq->size && pq->compare(pq->elements[left], pq->elements[smallest]) < 0) {
            smallest = left;
        }
        if (right < pq->size && pq->compare(pq->elements[right], pq->elements[smallest]) < 0) {
            smallest = right;
        }
        if (smallest != i) {
            Node* temp = pq->elements[i];
            pq->elements[i] = pq->elements[smallest];
            pq->elements[smallest] = temp;
            i = smallest;
        } else {
            break;
        }
    }
    return min;
}

void freePriorityQueue(PriorityQueue* pq) {
    if (pq) {
        free(pq->elements);
        free(pq);
    }
}

int isEmpty(PriorityQueue* pq) {
    return pq->size == 0;
}
