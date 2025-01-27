#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wejsciewyjscie.h"
#include "utils.h"
#define MAX_PLAYERS 100
#define MAX_NAME_LENGTH 50


typedef struct {
    char name[MAX_NAME_LENGTH];
    int score;
} Player;

int pierwszex, pierwszey;
void pierwszePole(char input[1000], char board[ROWS][COLS], char boardr[ROWS][COLS]) {
    printf("Podaj współrzędne w formacie [r] WIERSZ KOLUMNA (np. r 3 5): ");
    fgets(input, 1000, stdin);  // Add this line to read input
    wspolzedne(input, board, &pierwszey, &pierwszex, stdin);
    board[pierwszex][pierwszey] = 'S';
}

void wybierzPierwszePole(char board[ROWS][COLS], char boardr[ROWS][COLS]) {
    board[pierwszey][pierwszex] = '0'; // Reset to '0' after placing bombs
    printf("Pole jest puste. Sprawdzam sąsiadów...\n");
    bialePole(pierwszey, pierwszex, board, boardr);
}

void placeFlag(int y, int x, char boardr[ROWS][COLS]) {
    if (x >= 0 && x < ROWS && y >= 0 && y < COLS) {
        if (boardr[x][y] == '#') {
            boardr[x][y] = 'F';
        }
    }
}

void revealBombs(char board[ROWS][COLS], char boardr[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (board[i][j] == 'B') {
                boardr[i][j] = 'B';
            }
        }
    }
}

void checkForWin(char boardr[ROWS][COLS], int *koniec) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (boardr[i][j] == '#') {
                return;  // There are still unrevealed cells, so return early
            }
        }
    }
    *koniec = 2;  // No unrevealed cells left, player has won
}

void wybierzPole(char input[1000], char board[ROWS][COLS], char boardr[ROWS][COLS], FILE *file, int *koniec) {
    int x, y;

    if (file == stdin) {
        printf("Podaj akcję w formacie WIERSZxKOLUMNA (np. 3 x 5) lub 'f'/'r' przed współrzędnymi: ");
        fgets(input, 1000, stdin);  // Use the actual size of the array
    }

    char action = input[0];
    if (action == 'r' || action == 'f') {
        wspolzedne(input + 1, board, &y, &x, file);  // Skip the action character
    } else {
        wspolzedne(input, board, &y, &x, file);
    }

    if (action == 'f') {
        placeFlag(y, x, boardr);
    } else if (action == 'r') {
        if (board[x][y] == 'B') {
            *koniec = 1;
            revealBombs(board, boardr);  // Reveal all bombs
        } else if (board[x][y] == '0') {  // Check for '0' (char) instead of 0 (int)
            bialePole(y, x, board, boardr);
        } else {
            boardr[x][y] = board[x][y];
        }
    } else {
        if (board[x][y] == 'B') {
            *koniec = 1;
            revealBombs(board, boardr);  // Reveal all bombs
        } else if (board[x][y] == '0') {  // Check for '0' (char) instead of 0 (int)
            bialePole(y, x, board, boardr);
        } else {
            boardr[x][y] = board[x][y];
        }
    }
    printBoardr(boardr);
    checkForWin(boardr, koniec);
}

void printBoard(char board[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
}

void printBoardr(char board[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
                printf("%c ", board[i][j]);

        }
        printf("\n");
    }
}



void zapisGraczy(int score) {
    char name[100];  // Bufor na nazwę użytkownika (max 99 znaków + 1 znak końca '\0')

    printf("Podaj swoją nazwę: ");
    if (fgets(name, sizeof(name), stdin)) {  // Wczytuje dane do momentu końca linii lub przekroczenia bufora
        // Usuń znak nowej linii, jeśli istnieje
        name[strcspn(name, "\n")] = 0;
    }

    Player players[MAX_PLAYERS];
    int playerCount = 0;

    // Read current players from the file
    FILE *file = fopen("ListaGraczy.txt", "r");
    if (file != NULL) {
        while (fscanf(file, "%s %d", players[playerCount].name, &players[playerCount].score) == 2) {
            playerCount++;
        }
        fclose(file);
    }

    // Add the new player
    strcpy(players[playerCount].name, name);
    players[playerCount].score = score;
    playerCount++;

    // Sort players by score in descending order
    for (int i = 0; i < playerCount - 1; i++) {
        for (int j = i + 1; j < playerCount; j++) {
            if (players[j].score > players[i].score) {
                Player temp = players[i];
                players[i] = players[j];
                players[j] = temp;
            }
        }
    }

    // Write the top 5 players back to the file
    file = fopen("ListaGraczy.txt", "w");
    if (file != NULL) {
        for (int i = 0; i < playerCount && i < 5; i++) {
            fprintf(file, "%s %d\n", players[i].name, players[i].score);
        }
        fclose(file);
    }
}

void wyswietlListeGraczy() {
    const char *filename = "ListaGraczy.txt";
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Nie można otworzyć pliku: %s\n", filename);
        return;
    }

    char line[256];
    printf("Lista 5 najlepszych graczy:\n");
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }

    fclose(file);
}

int punkty(char boardr[ROWS][COLS], int mnoznik) {

    int score = 0;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (boardr[i][j] != '#' && boardr[i][j] != 'F' && boardr[i][j] != 'B') {
                score +=mnoznik;
            }
        }
    }
    return score;
}


