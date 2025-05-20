
#include "pathfinder.h"

int totalStates = 0; // Global variable to count total states generated

// Fungsi untuk membuat node baru
Node* createNode(Board* state, int g, int h, Node* parent, Move* move) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        fprintf(stderr, "Memory allocation failed for new node\n");
        exit(EXIT_FAILURE);
    }
    newNode->state = copyBoard(state);
    newNode->g = g;
    newNode->h = h;
    newNode->f = g + h;
    newNode->parent = parent;
    if (move) {
        newNode->move = *move;
    } else {
        newNode->move.pieceId = '\0';
        newNode->move.direction = '\0';
        newNode->move.steps = 0;
    }
    return newNode;
}

// Fungsi untuk mengecek apakah suatu state sudah dieksplorasi
bool isStateExplored(HashSet* exploredStates, Board* state) {
    return containsInHashSet(exploredStates, state);  // Mengecek apakah state sudah ada di HashSet
}

// Fungsi untuk merekonstruksi path dari goal node ke start node
void reconstructPath(Board* startNode, Node* goalNode) {
    Node* current = goalNode;
    Move* moves = (Move*)malloc(sizeof(Move) * 100);
    int moveCount = 0;

    while (current != NULL) {
        if (current->move.pieceId != '\0') {
            moves[moveCount++] = current->move;
           
            
        }
        current = current->parent;
    }
    printf("Papan Awal:\n");
    printBoard(startNode);
    printf("\n");

    printf("Gerakan:\n");
    Board* tempBoard = copyBoard(startNode);
    for (int i = moveCount - 1; i >= 0; i--) {
        // Cetak informasi gerakan
        if(moves[i].direction == 'U'){
            printf("Gerakan %d: Piece '%c' geser ke Atas sebanyak %d steps\n", moveCount - i, moves[i].pieceId, moves[i].steps);
        }else if(moves[i].direction == 'D'){
            printf("Gerakan %d: Piece '%c' geser ke Bawah sebanyak %d steps\n", moveCount - i, moves[i].pieceId, moves[i].steps);
        }else if(moves[i].direction == 'L'){
            printf("Gerakan %d: Piece '%c' geser ke Kiri sebanyak %d steps\n", moveCount - i, moves[i].pieceId, moves[i].steps);
        }else if(moves[i].direction == 'R'){
            printf("Gerakan %d: Piece '%c' geser ke Kanan sebanyak %d steps\n", moveCount - i, moves[i].pieceId, moves[i].steps);
        }
        // printf("Gerakan %d: Piece '%c' geser ke %c sebanyak %d steps\n", moveCount - i, moves[i].pieceId, moves[i].direction, moves[i].steps);
    

        // Gerakkan piece sesuai dengan langkah yang disimpan
        movePiece(tempBoard, moves[i].pieceId, moves[i].direction, moves[i].steps);

        // Cetak papan setelah setiap langkah
        printBoard(tempBoard);  // Menampilkan keadaan papan setelah langkah
        printf("\n");
    
    }
    
    freeBoard(tempBoard);
    free(moves);
}

// Heuristik: Menghitung Manhattan Distance untuk keluar
int heuristic1(const Board *board) {
    int distance = 0;
    for (int i = 0; i < board->numPieces; i++) {
        Piece *piece = &board->pieces[i];
        if (piece->id == board->primaryPieceID) {
            distance += abs(piece->row - board->exit_row) + abs(piece->col - board->exit_col);
        }
    }
    return distance;
}

// Fungsi untuk menghasilkan state berikutnya dari papan
Board** generateNextStates(const Board *board, int *nextStatesCount, Move* moves) {
    int capacity = 10000;
    Board** nextStates = (Board**)malloc(sizeof(Board*) * capacity); 
    *nextStatesCount = 0;
    if (!nextStates) {
        fprintf(stderr, "Memory allocation failed for nextStates\n");
        return NULL;
    }

    // printf("Debug: starting to generate next states\n");
    // printf("Debug: board->numPieces: %d\n", board->numPieces);
    for (int i = 0; i < board->numPieces; i++) {
        Piece *piece = &board->pieces[i];
        // printf("Debug: piece %c at (%d, %d)\n", piece->id, piece->row, piece->col);
    }
    for (int i = 0; i < board->numPieces; i++) {
        // printf("Debug: Generating next states for piece %c\n", board->pieces[i].id);
        
        Piece *piece = &board->pieces[i];
        for (int j = 0; j < 4; j++) {
            char direction = "UDLR"[j];

            // printf("Debug: Checking direction %c\n", direction);
            for (int steps = 1; steps <= 3; steps++) {
                if (canMove(board, piece, direction, steps)) {
                    // printf("Debug: Can move piece %c in direction %c by %d steps\n", piece->id, direction, steps);
                    if (*nextStatesCount >= capacity) {
                        capacity *= 2;
                        Board** temp = (Board**)realloc(nextStates, sizeof(Board*) * capacity);
                        if (!temp) {
                            fprintf(stderr, "Memory reallocation failed\n");
                            for (int k = 0; k < *nextStatesCount; k++) {
                                freeBoard(nextStates[k]);
                            }
                            free(nextStates);
                            return NULL;
                        }
                        nextStates = temp;
                    }
                    Board* newState = copyBoard(board);
                    movePiece(newState, piece->id, direction, steps);
                    // printf("Debug: Moving piece %c in direction %c by %d steps\n", piece->id, direction, steps);
                    nextStates[*nextStatesCount] = newState;
                    if (*nextStatesCount >= capacity) {
                        moves = realloc(moves, sizeof(Move) * capacity); // Sesuaikan kapasitas moves
                        if (!moves) {
                            fprintf(stderr, "Memory reallocation failed for moves\n");
                            for (int k = 0; k < *nextStatesCount; k++) {
                                freeBoard(nextStates[k]);
                            }
                            free(nextStates);
                            return NULL;
                        }
                     } // Menyesuaikan dengan kapasitas baru
                    moves[*nextStatesCount].pieceId = piece->id;
                    moves[*nextStatesCount].direction = direction;
                    moves[*nextStatesCount].steps = steps;
                    (*nextStatesCount)++;
                    // printf("Debug: nextStatesCount: %d\n", *nextStatesCount);
                }
            }
        }
    }
    return nextStates;
}

// Fungsi untuk menghasilkan child nodes berdasarkan parent
Node** generateChildNodes(Node* parent, int* count, int heuristicChoice) {
    int capacity = 10000;
    Node** children = malloc(sizeof(Node*) * capacity);
    // printf("Debug: Generating child \n");
    *count = 0;

    Move* moves = malloc(sizeof(Move) * capacity);
    int nextStatesCount;
    // printf("Debug: Generating next states\n");
    Board** nextBoards = generateNextStates(parent->state, &nextStatesCount, moves);
    // printf("Debug: Done Generating next states\n");

    for (int i = 0; i < nextStatesCount; i++) {
        if (*count >= capacity) {
            capacity *= 2;
            children = realloc(children, sizeof(Node*) * capacity);
        }
        int h = (heuristicChoice == 1) ? heuristic1(nextBoards[i]) : 0;
        Node* child = createNode(nextBoards[i], parent->g + 1, h, parent, &moves[i]);
        children[(*count)++] = child;
        freeBoard(nextBoards[i]);
    }

    free(moves);
    free(nextBoards);
    return children;
}

// Fungsi perbandingan untuk algoritma pathfinding
int compareGBFS(const Node* a, const Node* b) { return a->h - b->h; }
int compareUCS(const Node* a, const Node* b) { return a->g - b->g; }
int compareAStar(const Node* a, const Node* b) { return a->f - b->f; }

// Fungsi pencarian generik untuk berbagai algoritma pathfinding
Node* genericSearch(Board initialBoard, int heuristicChoice, int (*compare)(const Node*, const Node*)) {
    PriorityQueue* openList = createPriorityQueue(compare);
    // printf("Debug: Initializing openList\n");
    if (!openList) {
        fprintf(stderr, "Failed to create priority queue\n");
        return NULL;
    }
    HashSet* exploredStates = createHashSet();  // Menggunakan HashSet untuk melacak status yang sudah dieksplorasi
    // printf("Debug: Initializing exploredStates\n");

    int h = (heuristicChoice == 1) ? heuristic1(&initialBoard) : 0;
    Node* startNode = createNode(&initialBoard, 0, h, NULL, NULL);
    // printf("Debug: Creating startNode\n");
    enqueue(openList, startNode);
    // printf("Debug: Enqueuing startNode\n");
    // printf("Debug: openList at address %p\n", (void*)openList);

    while (!isEmpty(openList)) {
        Node* currentNode = dequeue(openList);
        // printf("Debug: Dequeuing currentNode\n");

        if (isSolved(currentNode->state)) {
            freePriorityQueue(openList);
            freeHashSet(exploredStates);  // Bebaskan memori HashSet
            return currentNode;
        }

        // Jika status belum dieksplorasi, simpan ke HashSet
        if (!isStateExplored(exploredStates, currentNode->state)) {
            // printf("Debug: Current node is not explored\n");
            insertToHashSet(exploredStates, currentNode->state);  // Menyimpan status yang dieksplorasi

            int childCount;
            // printf("Debug: Generating child nodes\n");
            Node** children = generateChildNodes(currentNode, &childCount, heuristicChoice);
            // printf("Debug: Done Generating child nodes\n");

            for (int i = 0; i < childCount; i++) {
                enqueue(openList, children[i]);
                // printf("Debug: Enqueuing child node\n");
            }
            free(children);
        } else {
            freeBoard(currentNode->state);  // Bebaskan memori jika status sudah dieksplorasi
        }
    }

    freePriorityQueue(openList);
    freeHashSet(exploredStates);  // Bebaskan memori HashSet
    return NULL;
}

// Implementasi algoritma pathfinding tertentu
Node* greedyBestFirstSearch(Board initialBoard, int heuristicChoice) {
    printf("Greedy Best-First Search with heuristic %d\n", heuristicChoice);
    return genericSearch(initialBoard, heuristicChoice, compareGBFS);
}

Node* uniformCostSearch(Board initialBoard) {
    return genericSearch(initialBoard, 0, compareUCS);
}

Node* aStarSearch(Board initialBoard, int heuristicChoice) {
    return genericSearch(initialBoard, heuristicChoice, compareAStar);
}
