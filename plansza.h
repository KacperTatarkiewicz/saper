#ifndef PLANSZA_H
#define PLANSZA_H
#include <stdio.h>

extern int COLS, ROWS;
void initializeBoard(char board[ROWS][COLS]);
void polaPoczatkowe(char board[ROWS][COLS], int i, int j);
void placeBombs(char board[ROWS][COLS], int n);
void numeracjaPol(char board[ROWS][COLS]);
void poziom(int *ROWS, int *COLS, int argc, char *argv[], int *bomby, FILE **file, int *mnoznik);
void initializeMystery(char board[ROWS][COLS]);
void kopiujPlansze(FILE *file, char board[ROWS][COLS]);


#endif // PLANSZA_H