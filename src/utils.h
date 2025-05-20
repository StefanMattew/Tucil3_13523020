#ifndef UTILS_H
#define UTILS_H

#include "board.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

void trim(char *str);
Board* loadBoardFromFile(const char *filename);
void printSolution(const Move *solution,int length); 

bool loadAndInitializeBoard(const char *filename, Board *initialBoard);
#endif