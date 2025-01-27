//
// Created by Kacper on 07.01.2025.
//
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "string.h"

int sprawdzSasiadow(int COLS, int ROWS, char board[ROWS][COLS], int i, int j, char pole) {
    int licznik = 0;
    if (i-1 >= 0 && j-1 >= 0 && board[i-1][j-1] == pole) licznik++;
    if (i-1 >= 0 && board[i-1][j] == pole) licznik++;
    if (i-1 >= 0 && j+1 < COLS && board[i-1][j+1] == pole) licznik++;
    if (j-1 >= 0 && board[i][j-1] == pole) licznik++;
    if (j+1 < COLS && board[i][j+1] == pole) licznik++;
    if (i+1 < ROWS && j-1 >= 0 && board[i+1][j-1] == pole) licznik++;
    if (i+1 < ROWS && board[i+1][j] == pole) licznik++;
    if (i+1 < ROWS && j+1 < COLS && board[i+1][j+1] == pole) licznik++;
    return licznik;
}

void wspolzedne(char input[1000], char board[ROWS][COLS], int *x, int *y, FILE *file) {
    char *token;
    input[strcspn(input, "\n")] = 0;

    token = strtok(input, " "); // Split only on spaces

    // Check for optional "r"
    if (token != NULL && strcmp(token, "r") == 0) {
        token = strtok(NULL, " "); // Skip "r"
    }

    if (token != NULL) {
        *x = atoi(token) - 1; // Parse row
        token = strtok(NULL, " "); // Get column
        if (token != NULL) {
            *y = atoi(token) - 1;
            if (*x >= 0 && *x < ROWS && *y >= 0 && *y < COLS) {
//              printf("Współrzędne: %d %d\n", *x, *y);
                return;
            } else {
                printf("Nieprawidłowe współrzędne.\n");
                exit(EXIT_FAILURE);
            }
        }
    }
    printf("Nieprawidłowy format. Użyj np. \"r 3 5\" lub \"3 5\".\n");
    exit(EXIT_FAILURE);
}

void bialePole(int x, int y, char board[ROWS][COLS], char boardr[ROWS][COLS]) {
    // Boundary check
    if (x < 0 || x >= ROWS || y < 0 || y >= COLS) return;

    // Skip if already revealed
    if (boardr[x][y] != '#') return;

    // Reveal the cell
    boardr[x][y] = board[x][y];

    // Stop recursion if the cell is not '0'
    if (board[x][y] != '0') return;

    // Recursively reveal all 8 neighbors
    bialePole(x-1, y-1, board, boardr);
    bialePole(x-1, y,   board, boardr);
    bialePole(x-1, y+1, board, boardr);
    bialePole(x,   y-1, board, boardr);
    bialePole(x,   y+1, board, boardr);
    bialePole(x+1, y-1, board, boardr);
    bialePole(x+1, y,   board, boardr);
    bialePole(x+1, y+1, board, boardr);
}