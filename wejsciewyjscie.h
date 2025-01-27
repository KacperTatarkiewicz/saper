//
// Created by Kacper on 10.01.2025.
//

#ifndef WEJSCIE_H
#define WEJSCIE_H
#include <stdio.h>

extern int ROWS, COLS;
void printBoard(char board[ROWS][COLS]);
void pierwszePole(char input[1000], char board[ROWS][COLS], char boardr[ROWS][COLS]);
void wybierzPole(char input[1000], char board[ROWS][COLS], char boardr[ROWS][COLS], FILE *file, int *koniec);
void printBoardr(char board[ROWS][COLS]);
void wybierzPierwszePole(char board[ROWS][COLS], char boardr[ROWS][COLS]);
int punkty(char boardr[ROWS][COLS], int mnoznik);
void zapisGraczy(int score);
void wyswietlListeGraczy();


#endif //WEJSCIE_H
