//
// Created by Kacper on 07.01.2025.
//

#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>

extern int COLS, ROWS;
int sprawdzSasiadow(int COLS,int ROWS,char board[ROWS][COLS],int i,int j, char pole);
void wspolzedne(char input[1000], char board[ROWS][COLS], int *x, int *y ,FILE *file);
void bialePole(int x, int y, char board[ROWS][COLS], char boardr[ROWS][COLS]);

#endif //UTILS_H
