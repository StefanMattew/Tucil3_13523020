#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void trim(char *str) {
    int start = 0;
    while (isspace((unsigned char)str[start])) start++;
    int end = strlen(str) - 1;
    while (end > start && isspace((unsigned char)str[end])) end--;
    for (int i = 0; i <= end - start; i++) str[i] = str[start + i];
    str[end - start + 1] = '\0';
}

Board *loadBoardFromFile(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    printf("Failed to open file: %s\n", filename);
    return NULL;
  }

  Board *board = malloc(sizeof(Board));
  if (!board) {
    printf("Failed to allocate memory for board\n");
    fclose(file);
    return NULL;
  }

  if (fscanf(file, "%d %d\n", &board->rows, &board->cols) != 2) {
    printf("Error reading board dimensions\n");
    fclose(file);
    free(board);
    return NULL;
  }

  if (fscanf(file, "%d\n", &board->numPieces) != 1) {
    printf("Error reading number of pieces\n");
    fclose(file);
    free(board);
    return NULL;
  }
  board->numPieces +=1;

  board->grid = malloc(board->rows * sizeof(char *));
  for (int i = 0; i < board->rows; i++) {
    board->grid[i] = malloc((board->cols + 1) * sizeof(char));
    for (int j = 0; j < board->cols; j++) {
      board->grid[i][j] = '.';
    }
    board->grid[i][board->cols] = '\0';
  }

  board->pieces = malloc(board->numPieces * sizeof(Piece));
  if (!board->pieces) {
    for (int i = 0; i < board->rows; i++) free(board->grid[i]);
    free(board->grid);
    free(board);
    fclose(file);
    return NULL;
  }

  char line[256];
  int fileLineIndex = 0;
  int gridLineIndex = 0;
  int pieceCount = 0;
  int pieceMap[256] = {0};
  board->exit_row = -1;
  board->exit_col = -2; // Nilai khusus untuk menandakan belum ditemukan
  int kPosition = -1;    // Inisialisasi posisi K

  while (fgets(line, sizeof(line), file)) {
    size_t len = strlen(line);
    if (len > 0 && line[len - 1] == '\n') {
      line[len - 1] = '\0';
      len--;
    }

    // Cari posisi 'K' *sebelum* trim jika ini baris pertama
    kPosition = -1; // Reset posisi K setiap baris
    if (fileLineIndex == 0) {
      for (int i = 0; i < strlen(line); i++) { // Gunakan strlen(line) disini
        if (line[i] == 'K') {
          kPosition = i;
          break;
        }
      }
    }

    trim(line); // Trim setelah mencari K (kecuali baris pertama)

    if (fileLineIndex != 0) { // Cari K setelah trim untuk baris selain yang pertama
      kPosition = -1;
      for (int i = 0; i < len; i++) {
        if (line[i] == 'K') {
          kPosition = i;
          break;
        }
      }
    }

    if (kPosition != -1) { // Jika 'K' ditemukan
      if (fileLineIndex == 0) { // Jika ini baris pertama
        board->exit_row = -1;
        board->exit_col = kPosition; // kPosition sudah benar
      } else if (kPosition == 0) { // Jika 'K' di kiri
        board->exit_row = fileLineIndex;
        board->exit_col = -1;
      } else if (kPosition == len) { // Jika 'K' di kanan
        board->exit_row = fileLineIndex;
        board->exit_col = board->cols; // Atau board->cols, tergantung kebutuhan Anda
      } else if (fileLineIndex == board->rows - 1) { // Jika 'K' di baris terakhir (bawah)
        board->exit_row = board->rows;
        board->exit_col = kPosition;
      } else {
        board->exit_row = fileLineIndex;
        board->exit_col = kPosition;
      }

      // Hapus 'K' dari line
      for (int i = kPosition; i < len - 1; i++) {
        line[i] = line[i + 1];
      }
      if (kPosition != -1) {
        line[len - 1] = '.';
        len--;
      }
    }

    // Simpan line ke grid jika masih dalam batas grid dan bukan baris pertama
    if (gridLineIndex < board->rows && fileLineIndex != 0) {
      for (int j = 0; j < board->cols; j++) {
        board->grid[gridLineIndex][j] = (j < len) ? line[j] : '.';
      }
      board->grid[gridLineIndex][board->cols] = '\0';
      gridLineIndex++;
    } else if (gridLineIndex < board->rows && fileLineIndex == 0 && kPosition == -1) {
      // Jika baris pertama dan 'K' tidak ditemukan, simpan seperti biasa
      for (int j = 0; j < board->cols; j++) {
        board->grid[gridLineIndex][j] = (j < len) ? line[j] : '.';
      }
      board->grid[gridLineIndex][board->cols] = '\0';
      gridLineIndex++;
    }

    fileLineIndex++;
  }

  fclose(file);

  if (board->exit_row == -1 && board->exit_col == -2) {
    printf("Error: Exit 'K' not found in any line\n");
    for (int i = 0; i < board->rows; i++) free(board->grid[i]);
    free(board->grid);
    free(board->pieces);
    free(board);
    return NULL;
  }

  // Proses pieces
  for (int i = 0; i < board->rows; i++) {
    for (int j = 0; j < board->cols; j++) {
      char c = board->grid[i][j];
      if (c == '.' || c == 'K') continue;

      if (!pieceMap[(unsigned char)c]) {
        Piece p;
        p.id = c;
        p.row = i;
        p.col = j;
        p.size = 1;
        p.orientation = 0;
        p.isPrimary = (c == 'P');

        board->pieces[pieceCount] = p;
        pieceMap[(unsigned char)c] = pieceCount + 1;

        if (p.isPrimary) {
          board->primaryPieceID = c;
          board->primaryIndex = pieceCount;
        }
        pieceCount++;
      } else {
        board->pieces[pieceMap[(unsigned char)c] - 1].size++;
      }
    }
  }

  board->numPieces = pieceCount;

  // Deteksi orientasi
  for (int i = 0; i < board->numPieces; i++) {
    Piece *p = &board->pieces[i];
    if (p->row + 1 < board->rows && board->grid[p->row + 1][p->col] == p->id) {
      p->orientation = 'V';
    } else {
      p->orientation = 'H';
    }
  }

  board->solution = NULL;
  board->solutionLength = 0;
  return board;
}


void printSolution(const Move *solution, int length) {
    for (int i = 0; i < length; i++) {
        printf("Move piece %c %c %d\n", solution[i].pieceId, solution[i].direction, solution[i].steps);
    }
}


bool loadAndInitializeBoard(const char *filename, Board *initialBoard) {
    Board *board = loadBoardFromFile(filename);
    if (!board) {
        printf("Failed to load board from file: %s\n", filename);
        return false; // Failed to load
    }

    // Copy data from board to initialBoard
    initialBoard->rows = board->rows;
    initialBoard->cols = board->cols;
    initialBoard->numPieces = board->numPieces;
    initialBoard->exit_row = board->exit_row;
    initialBoard->exit_col = board->exit_col;
    initialBoard->primaryPieceID = board->primaryPieceID;
    initialBoard->primaryIndex = board->primaryIndex;

    // Allocate and copy grid
    initialBoard->grid = malloc(initialBoard->rows * sizeof(char *));
    if (!initialBoard->grid) {
        fprintf(stderr, "Memory allocation error for initialBoard.grid\n");
        freeBoard(board);
        return false;
    }
    for (int i = 0; i < initialBoard->rows; i++) {
        initialBoard->grid[i] = malloc((initialBoard->cols + 1) * sizeof(char));
        if (!initialBoard->grid[i]) {
            fprintf(stderr, "Memory allocation error for initialBoard.grid[%d]\n", i);
            for (int j = 0; j < i; j++) free(initialBoard->grid[j]);
            free(initialBoard->grid);
            freeBoard(board);
            return false;
        }
        strcpy(initialBoard->grid[i], board->grid[i]);
    }
    initialBoard->pieces = malloc(initialBoard->numPieces * sizeof(Piece));
    if (!initialBoard->pieces) {
        fprintf(stderr, "Memory allocation error for initialBoard.pieces\n");
        for (int i = 0; i < initialBoard->rows; i++) free(initialBoard->grid[i]);
        free(initialBoard->grid);
        freeBoard(board);
        return false;
    }
    memcpy(initialBoard->pieces, board->pieces, initialBoard->numPieces * sizeof(Piece));

    freeBoard(board); // Free the temporary board
    return true; // Successfully loaded and initialized
}

// void printBoard(const Board *board) {
//     printf("Board Configuration:\n");
//     for (int i = 0; i < board->rows; i++) {
//         for (int j = 0; j < board->cols; j++) {
//             printf("%c ", board->grid[i][j]);
//         }
//         printf("\n");
//     }
// }

// void freeBoard(Board *board) {
//     if (board) {
//         // Free the grid
//         if (board->grid) {
//             for (int i = 0; i < board->rows; i++) {
//                 free(board->grid[i]);
//             }
//             free(board->grid);
//         }
//         // Free the pieces array
//         if (board->pieces) {
//             free(board->pieces);
//         }
//         // Free the solution (if it exists)
//         if (board->solution) {
//             free(board->solution);
//         }
//         free(board); // Free the Board struct itself
//     }
// }