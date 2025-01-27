#include <stdio.h>
#include "plansza.h"
#include "wejsciewyjscie.h"
#include <string.h>



int ROWS, COLS;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Użycie: %s [-p <poziom_trudności>(łatwy, średni, trudny lub niestandardowy) [ilość_wierszy] [ilość_kolumn] [ilość bomb]] [-f <nazwa_pliku>]\n", argv[0]);
        return 1;
    }

    int mnoznik = 1;

    FILE *file = NULL;
    int bomby;
    char input[1000];  // Bufor na wejście użytkownika
    int koniec = 0;
    // Ustaw rozmiar planszy, ilość bomb i otwórz plik, jeśli podano opcję "-f"
    poziom(&ROWS, &COLS, argc, argv, &bomby, &file, &mnoznik);

    char board[ROWS][COLS];
    char boardr[ROWS][COLS];  // Tablica widoczna dla gracza (tablica rzeczywista)
    initializeMystery(boardr);  // Inicjalizacja planszy widocznej dla gracza
    initializeBoard(board);      // Inicjalizacja planszy z odpowiedziami

    // Jeśli plik nie został podany, przejdź do trybu interaktywnego
    if (file == NULL) {
        pierwszePole(input, board, boardr);  // Ustaw pierwsze pole

        placeBombs(board, bomby);  // Umieść bomby na planszy
        numeracjaPol(board);       // Numeracja pól

        wybierzPierwszePole(board, boardr);  // Pozwól użytkownikowi wybrać pierwsze pole
        printBoardr(boardr);  // Wyświetl planszę
        // Pętla w trybie interaktywnym
        while (koniec == 0) {
            wybierzPole(input, board, boardr, stdin,&koniec);  // Pobierz pole od użytkownika
        }
    } else {
        // Tryb plikowy: wczytaj komendy z pliku
        char buffer[1000];
        int line = 0;
        kopiujPlansze(file, board);  // Skopiuj planszę z pliku do tablicy `board`
        rewind(file);  // Przenieś wskaźnik pliku na początek

        while (fgets(buffer, sizeof(buffer), file) && koniec == 0) {
            // Pomiń linie, które należą do planszy (te, które już wczytaliśmy)

            if (line < ROWS) {
                line++;
                continue;

            }


            // Wykonaj polecenie wczytane z pliku
            printf("Wczytano polecenie: %s", buffer);  // Debug: pokaż polecenie
            wybierzPole(buffer, board, boardr, file, &koniec);  // Wykonaj polecenie

        }
//        printBoard(board);  // Wyświetl planszę po zakończeniu gry
//        printBoardr(boardr);  // Wyświetl planszę widoczną dla gracza


        fclose(file);  // Zamknij plik po zakończeniu
    }
    int score = punkty(boardr, mnoznik) ;  // Oblicz wynik
    if (koniec == 2) {
        printf("Gratulacje! Wygrałeś!\n");
    } else {
    printf("Trafiłeś na bombę! Koniec gry.\n");
    }
    printf("Twój wynik to: %d\n", score);
    zapisGraczy(score);  // Zapisz wynik gracza
    wyswietlListeGraczy();  // Wyświetl listę graczy



    return 0;
}