#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include "plansza.h"
#include "utils.h"

void poziom(int *ROWS, int *COLS, int argc, char *argv[], int *bomby, FILE **file, int *mnoznik) {
    int opt;  // Variable to store getopt options
    char *filename = NULL;  // Variable to store the filename

    // Process options using getopt
    while ((opt = getopt(argc, argv, "p:f:")) != -1) {
        switch (opt) {
            case 'p':  // Difficulty level
                if (strcmp(optarg, "łatwy") == 0) {
                    *ROWS = 9;
                    *COLS = 9;
                    *bomby = 10;
                    *mnoznik = 1;
                } else if (strcmp(optarg, "średni") == 0) {
                    *ROWS = 16;
                    *COLS = 16;
                    *bomby = 40;
                    *mnoznik = 2;
                } else if (strcmp(optarg, "trudny") == 0) {
                    *ROWS = 16;
                    *COLS = 30;
                    *bomby = 99;
                    *mnoznik = 3;
                } else if (strcmp(optarg, "niestandardowy") == 0) {
                    *ROWS = atoi(argv[3]);
                    *COLS = atoi(argv[4]);
                    *bomby = atoi(argv[5]);

                    if (*bomby > *ROWS * *COLS) {
                        fprintf(stderr, "Zbyt dużo bomb.\n");
                        exit(EXIT_FAILURE);
                    }
                    else if (*ROWS < 3 || *COLS < 3 || *bomby < 1) {
                        fprintf(stderr, "Wartości muszą być większe od 0.\n");
                        exit(EXIT_FAILURE);
                    }
                } else {
                    fprintf(stderr, "Nieprawidłowy poziom trudności: %s\n", optarg);
                    exit(EXIT_FAILURE);
                }
                break;

            case 'f':  // File mode with filename
                filename = optarg;
                break;

            default:  // Invalid option
                fprintf(stderr, "Użycie: %s [-p <poziom_trudności>(łatwy, średni, trudny lub niestandardowy) [ilość_wierszy] [ilość_kolumn] [ilość bomb]] [-f <nazwa_pliku>]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (filename != NULL) {
        *file = fopen(filename, "r");
        if (*file == NULL) {
            fprintf(stderr, "Nie można otworzyć pliku: %s\n", filename);
            exit(EXIT_FAILURE);
        }

        // Reading the number of rows (ROWS) and columns (COLS)
        char buffer[256];
        int cols_count = 0;
        *ROWS = 0;  // Initialize ROWS to 0
        *COLS = 0;  // Initialize COLS to 0

        while (fgets(buffer, sizeof(buffer), *file)) {
            // Check if the line starts with 'f'
            if (buffer[0] == 'f' || buffer[0] == 'r') {
                break;
            }
            if (cols_count == 0) {
                // Count columns based on the number of numbers in the row
                char *token = strtok(buffer, " ");
                while (token != NULL) {
                    (*COLS)++;
                    token = strtok(NULL, " ");
                }
                cols_count++;
            }

            // Increment the row count
            (*ROWS)++;
        }

        // Set the number of columns based on the last row
    }
}



void initializeBoard(char board[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            board[i][j] = '0'; // Initialize to '0' instead of 0
        }
    }
}

void initializeMystery(char board[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            board[i][j] = '#';
        }
    }
}


void placeBombs(char board[ROWS][COLS], int n) {
    srand(time(NULL));
    int placedBombs = 0;
    while (placedBombs < n) {
        int row = rand() % ROWS;
        int col = rand() % COLS;
        // Check if cell is '0' and no adjacent 'S' (initial cell)
        if (board[row][col] == '0' && sprawdzSasiadow(COLS, ROWS, board, row, col, 'S') == 0) {
            board[row][col] = 'B'; // Bomb represented as 'B'
            placedBombs++;
        }
    }
}

void numeracjaPol(char board[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (board[i][j] == '0') {
                int count = sprawdzSasiadow(COLS, ROWS, board, i, j, 'B');
                board[i][j] = count + '0';  // Convert to ASCII
            }
        }
    }
}

void kopiujPlansze(FILE *file, char board[ROWS][COLS]) {
    char buffer[256];
    int row = 0;
    rewind(file);
    while (fgets(buffer, sizeof(buffer), file) != NULL && row < ROWS) {
        char *token = strtok(buffer, " \n");
        int col = 0;
        while (token != NULL && col < COLS) {
            if (strcmp(token, "B") == 0) {
                board[row][col] = 'B'; // Assign bomb
            } else {
                board[row][col] = *token; // Assign number as char
            }
            token = strtok(NULL, " \n");
            col++;
        }
        row++;
    }
}
