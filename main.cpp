#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Board {
   public:
    /// Tablica zawierająca komórki planszy
    /// 0 - pole wolne
    /// 1 - żeton gracza pierwszego
    /// 2 - żeton gracza drugiego
    short cells[36] = {0};

    /// Określa którego gracza jest teraz ruch
    short player = 1;

   private:
    /// Zwraca pierwszy element podanej ćwiartki
    /// W innym wypadku -1
    int decodeQuarterId(char quarterId) {
        if (quarterId == 'q')
            return 0;
        else if (quarterId == 'w')
            return 3;
        else if (quarterId == 'a')
            return 18;
        else if (quarterId == 's')
            return 21;
        else
            return -1;
    }

    /// Zwraca indeks odpowiadający podanemu znakowi
    int decodeCellId(char cellId) {
        string keyString = "789   456   123";
        return keyString.find(cellId);
    }

    /// Dla wektora 6 lub 5 elementowego sprawdza czy żetony graczy się zgadzają
    int checkWin(vector<int> vect) {
        if (vect.size() == 6) {
            // Rozdziela 6 elementowy wektor na dwia 5 elementowe
            vector<int> v1 = vect;
            v1.erase(v1.begin());
            vector<int> v2 = vect;
            v2.pop_back();
            if (checkWin(v1) == 1 || checkWin(v2) == 1)
                return 1;
            else if (checkWin(v1) == 2 || checkWin(v2) == 2)
                return 2;
            else
                return 0;
        } else if (vect.size() == 5) {
            // Sprawdza czy wszystkie elementy wektora są równe
            for (int i = 1; i < 5; i++)
                if (cells[vect[i]] != cells[vect[0]]) return 0;
            return cells[vect[0]];

        } else
            return 0;
    }

    /// Sprawdza czy w danej komórce można postawić żeton
    bool canPlaceToken(int id) { return cells[id] == 0; }

    /// Odwraca odpowiednią ćwiartke zgodnie z ruchem wskazówek zegara
    /// Ćwiartki są numerowane od 0 do 3
    void rotateCW(char quarterId) {
        // Okreslenie pierwszego elementu ćwiartki
        int origin = decodeQuarterId(quarterId);
        if (origin == -1) return;

        // Podmiana elementów na rogach planszy
        short buffor = cells[origin];
        cells[origin] = cells[origin + 12];
        cells[origin + 12] = cells[origin + 14];
        cells[origin + 14] = cells[origin + 2];
        cells[origin + 2] = buffor;

        // Podmiana elementów na krawędziach planszy
        origin++;
        buffor = cells[origin];
        cells[origin] = cells[origin + 5];
        cells[origin + 5] = cells[origin + 12];
        cells[origin + 12] = cells[origin + 7];
        cells[origin + 7] = buffor;
    }

    /// Odwraca odpowiednią ćwiartke przeciwnie do ruchu wskazówek zegara
    /// Ćwiartki są numerowane od 0 do 3
    void rotateCCW(char quarterId) {
        // Okreslenie pierwszego elementu ćwiartki
        int origin = decodeQuarterId(quarterId);
        if (origin == -1) return;

        // Podmiana elementów na rogach planszy
        short buffor = cells[origin];
        cells[origin] = cells[origin + 2];
        cells[origin + 2] = cells[origin + 14];
        cells[origin + 14] = cells[origin + 12];
        cells[origin + 12] = buffor;

        // Podmiana elementów na krawędziach planszy
        origin++;
        buffor = cells[origin];
        cells[origin] = cells[origin + 7];
        cells[origin + 7] = cells[origin + 12];
        cells[origin + 12] = cells[origin + 5];
        cells[origin + 5] = buffor;
    }

   public:
    /// Stawia żeton w odpowiedniej komórce
    /// NIE sprawdza czy komórka jest dostępna i nadpisuje wartość
    void placeToken(int id, short player) { cells[id] = player; }

    /// Zwraca token o podanym indeksie
    short getTokenId(int id) {
        if (cells[id] == 1)
            return 1;
        else if (cells[id] == 2)
            return 2;
        else
            return 0;
    }

    /// Wykonuje ruch
    /// Zwraca 1 przy powodzeniu i 0 przy niepowodzeniu
    bool makeAMove(char placeQuarterId, char placeCellId, char rotateQuarterId,
                   char rotateDirection) {
        // Obliczanie indeksu dla tablicy cell
        int id = decodeCellId(placeCellId) + decodeQuarterId(placeQuarterId);

        // Stawianie żetonu jeśli można
        if (canPlaceToken(id))
            placeToken(id, player);
        else
            return false;

        // Obracanie odpowiedniej ćwiartki
        if (tolower(rotateDirection) == 'z')
            rotateCW(rotateQuarterId);
        else if (tolower(rotateDirection) == 'x')
            rotateCCW(rotateQuarterId);
        return true;
    }

    /// Zwraca
    /// 0 - gdy nikt nie wygrał
    /// 1 - gdy wygrał pierwzy gracz
    /// 2 - gdy wygrał drugi gracz
    /// 3 - gdy wygrali obydwoje
    short winner() {
        // Zakłada ze nikt nie wygrał
        bool firstWon = false;
        bool secondWon = false;

        vector<int> vec;
        int w = 0;
        // sprawdzanie kolumn
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 6; j++) vec.push_back(j + (i * 6));
            w = checkWin(vec);
            if (!firstWon) firstWon = w == 1;
            if (!secondWon) secondWon = w == 2;
            vec.clear();
        }

        // sprawdzanie rzędów
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 6; j++) vec.push_back(i + (j * 6));
            w = checkWin(vec);
            if (!firstWon) firstWon = w == 1;
            if (!secondWon) secondWon = w == 2;
            vec.clear();
        }

        // sprawdzanie przekątnych
        for (int j = 0; j < 6; j++) vec.push_back((j * 7));
        w = checkWin(vec);
        if (!firstWon) firstWon = w == 1;
        if (!secondWon) secondWon = w == 2;
        vec.clear();
        for (int j = 0; j < 5; j++) vec.push_back(1 + (j * 7));
        w = checkWin(vec);
        if (!firstWon) firstWon = w == 1;
        if (!secondWon) secondWon = w == 2;
        vec.clear();
        for (int j = 0; j < 5; j++) vec.push_back(6 + (j * 7));
        w = checkWin(vec);
        if (!firstWon) firstWon = w == 1;
        if (!secondWon) secondWon = w == 2;
        vec.clear();

        // sprawdzanie przeciwnych przekątnych
        for (int j = 0; j < 6; j++) vec.push_back(5 + (j * 5));
        w = checkWin(vec);
        if (!firstWon) firstWon = w == 1;
        if (!secondWon) secondWon = w == 2;
        vec.clear();
        for (int j = 0; j < 5; j++) vec.push_back(4 + (j * 5));
        w = checkWin(vec);
        if (!firstWon) firstWon = w == 1;
        if (!secondWon) secondWon = w == 2;
        vec.clear();
        for (int j = 0; j < 5; j++) vec.push_back(11 + (j * 5));
        w = checkWin(vec);
        if (!firstWon) firstWon = w == 1;
        if (!secondWon) secondWon = w == 2;
        vec.clear();

        // if (firstWon && secondWon)
        //     return 3;
        // else if (firstWon)
        //     return 1;
        // else if (secondWon)
        //     return 2;
        // else
        //     return 0;

        return firstWon + (2 * secondWon);
    }
};

class Game {
   private:
    /// Imiona graczy
    string name1, name2;
    /// Zetony graczy
    char token1 = '\0', token2 = '\0';
    /// Czas graczy
    int time1 = 0, time2 = 0;
    /// Vector z kolejnymi ruchami graczy
    vector<Board> boards = {Board()};
    /// Określa czy rozgrywana jest gra pentago czy kolko i krzyżyk
    bool pentago = true;
    /// Liczba wykonanych ruchów
    int step = 0;

    /// Prywatne stałe uzywane do wyświetlania planszy
    const char TOPLINE[27] = {
        (char)0xC9, (char)0xCD, (char)0xCD, (char)0xCD, (char)0xCB, (char)0xCD,
        (char)0xCD, (char)0xCD, (char)0xCB, (char)0xCD, (char)0xCD, (char)0xCD,
        (char)0xCB, (char)0xCD, (char)0xCD, (char)0xCD, (char)0xCB, (char)0xCD,
        (char)0xCD, (char)0xCD, (char)0xCB, (char)0xCD, (char)0xCD, (char)0xCD,
        (char)0xBB, '\t',       '\0'};
    const char BOTTOMLINE[27] = {
        (char)0xC8, (char)0xCD, (char)0xCD, (char)0xCD, (char)0xCA, (char)0xCD,
        (char)0xCD, (char)0xCD, (char)0xCA, (char)0xCD, (char)0xCD, (char)0xCD,
        (char)0xCA, (char)0xCD, (char)0xCD, (char)0xCD, (char)0xCA, (char)0xCD,
        (char)0xCD, (char)0xCD, (char)0xCA, (char)0xCD, (char)0xCD, (char)0xCD,
        (char)0xBC, '\t',       '\0'};
    const char MIDLINE[27] = {
        (char)0xCC, (char)0xCD, (char)0xCD, (char)0xCD, (char)0xCE, (char)0xCD,
        (char)0xCD, (char)0xCD, (char)0xCE, (char)0xCD, (char)0xCD, (char)0xCD,
        (char)0xCE, (char)0xCD, (char)0xCD, (char)0xCD, (char)0xCE, (char)0xCD,
        (char)0xCD, (char)0xCD, (char)0xCE, (char)0xCD, (char)0xCD, (char)0xCD,
        (char)0xB9, '\t',       '\0'};
    const char VARIABLELINE[33] = {
        (char)0xBA, ' ',  '%', 'c', ' ', (char)0xBA, ' ', '%', 'c', ' ',
        (char)0xBA, ' ',  '%', 'c', ' ', (char)0xBA, ' ', '%', 'c', ' ',
        (char)0xBA, ' ',  '%', 'c', ' ', (char)0xBA, ' ', '%', 'c', ' ',
        (char)0xBA, '\t', '\0'};

    /// Włącza pauze
    void pause() {
        string inp;
        system("cls");
        cout << "\tPAUZA\n\n";
        cout << "Nacisnij dowlony przycisk aby kontynuowac..." << endl;
        getline(cin, inp);
        system("cls");
    }

    /// Włącza ustawienia
    void settings() {
        system("cls");
        printf("\tUSTAWIENIA:\n\n");
        printf("\t1. Zmien imiona\n");
        printf("\t2. Zmien zetony\n");
        printf("\t3. Zmien czas\n");
        printf("\t4. Wyjscie\n");
        while (true) {
            string inp;
            getline(cin, inp);
            system("cls");
            if (inp == "1") {
                name1.clear();
                name2.clear();
                scanNames();
                break;
            } else if (inp == "2") {
                token1 = '\0';
                token2 = '\0';
                scanTokens();
                break;
            } else if (inp == "3") {
                time1 = 0;
                time2 = 0;
                scanTime();
                break;
            } else if (inp == "4") {
                break;
            } else
                settings();
        }
    }

    /// Włącza pomoc
    void help() {
        string inp;
        system("cls");
        cout
            << "q,w,a,s - wybor czesci planszy odpowiednio: lewej gornej, "
               "prawej gornej, lewej dolnej, prawej dolnej\n1..9 - wybor pola "
               "na czesci planszy jak na klawiaturze numerycznej tj.1 to "
               "dolne lewe pole\nz, x - obrot odpowiednio: zgodnie z ruchem "
               "wskazowek zegara, odwrotnie do ruchu wskazowek\np - pauza\nu - "
               "undo\no - wczytanie predefiniowanej planszy\nm - wejscie do "
               "opcji\nh - wejscie do opisu programu\n\nRuch to najpierw wybor "
               "czesci planszy i pola (np.q5), a nastepnie wybor czesci "
               "planszy i obrotu (np.az)."
            << endl;

        cout << "Nacisnij dowlony przycisk aby kontynuowac..." << endl;
        getline(cin, inp);
    }

    /// Włącza przeglądanie rozgrywki
    void browse() {
        cout << "Mozesz teraz przegladac rozgrywke za pomoca komend\n\t\'a\' "
                "by sie cofnac i \n\t\'s\' by przejsc do nastepnego "
                "ruchu\n\t\'x\' aby wyjsc."
             << endl;
        int move = step;
        while (true) {
            string inp;
            getline(cin, inp);
            system("cls");
            if (inp == "a" || inp == "A") {
                if (move > 0) move--;
                printBoard(boards[move]);
            } else if (inp == "s" || inp == "S") {
                if (move < step) move++;
                printBoard(boards[move]);
            } else if (inp == "x" || inp == "X") {
                exit(0);
            }
        }
    }

    /// Główna pętla gry
    void play() {
        while (true) {
            printBoard(boards[step]);

            // Czas rozpoczęcia ruchu
            time_t startTime = time(NULL);
            // Wczytanie wejścia
            string inp;
            getline(cin, inp);
            // Czas zakończenia ruchu
            time_t endTime = time(NULL);

            // Aktualizuje czas gracza i sprawdza czy się nie skończył
            if (boards[step].player == 1) {
                time1 -= endTime - startTime;
                if (time1 <= 0) {
                    cout << "Czas gracza pierwszego sie skonczyl." << endl;
                    cout << "Wygrywa gracz drugi." << endl;
                    break;
                }
            } else if (boards[step].player == 2) {
                time2 -= endTime - startTime;
                if (time2 <= 0) {
                    cout << "Czas gracza drugiego sie skonczyl." << endl;
                    cout << "Wygrywa gracz pierwszy." << endl;
                    break;
                }
            }

            system("cls");
            // Walidacja wejścia
            if (inp == "q7") {
                while (1) cout << rand();
            }
            if (inp == "p" || inp == "P") {
                pause();
            } else if ((inp == "u" || inp == "U") && step > 0) {
                boards.pop_back();
                step--;
                // Ustawia plansze według ostatniego stanu
                for (int i = 0; i < 36; i++)
                    boards[step].cells[i] = boards[step - 1].cells[i];
            } else if (inp == "o" || inp == "O") {
                loadPredefinedState();
            } else if (inp == "m" || inp == "M") {
                settings();
            } else if (inp == "h" || inp == "H") {
                help();
            }
            // Kółko i krzyżyk
            else if (!pentago && inp.size() == 2) {
                // Walidacja ruchu
                if (tolower(inp[0]) == 'q' || tolower(inp[0]) == 'w' ||
                    tolower(inp[0]) == 'a' || tolower(inp[0]) == 's') {
                    // Wykonanie ruchu
                    if (boards[step].makeAMove(inp[0], inp[1], '\0', '\0')) {
                        // Sprawdzanie zwycięstwa
                        short w = boards[step].winner();
                        if (w == 1) {
                            cout << "Wygral gracz pierwszy " << name1 << endl;
                            break;
                        } else if (w == 2) {
                            cout << "Wygral gracz drugi " << name2 << endl;
                            break;
                        } else {
                            // Tworzenie nowej planszy na nastepny ruch
                            Board newBoard = Board();
                            for (int i = 0; i < 36; i++)
                                newBoard.cells[i] = boards[step].cells[i];

                            newBoard.player = boards[step].player % 2 + 1;
                            boards.push_back(newBoard);
                            step++;
                        }
                    } else {
                        cout << "Pole jest zajete" << endl;
                    }
                } else {
                    cout << "Niepoprawny ruch" << endl;
                    cout << "Wpisz \'h\' by wyswietlic pomoc." << endl;
                }
            }
            // Pentago
            else if (pentago && inp.size() == 5 && inp[2] == ' ') {
                // Walidacja ruchu
                if ((tolower(inp[0]) == 'q' || tolower(inp[0]) == 'w' ||
                     tolower(inp[0]) == 'a' || tolower(inp[0]) == 's') &&
                    (tolower(inp[3]) == 'q' || tolower(inp[3]) == 'w' ||
                     tolower(inp[3]) == 'a' || tolower(inp[3]) == 's') &&
                    (tolower(inp[4]) == 'z' || tolower(inp[4]) == 'x')) {
                    // Wykonanie ruchu
                    if (boards[step].makeAMove(inp[0], inp[1], inp[3],
                                               inp[4])) {
                        // Sprawdzanie zwycięstwa
                        short w = boards[step].winner();
                        if (w == 1) {
                            cout << "Wygral gracz pierwszy " << name1 << endl;
                            break;
                        } else if (w == 2) {
                            cout << "Wygral gracz drugi " << name2 << endl;
                            break;
                        } else {
                            // Tworzenie nowej planszy na nastepny ruch
                            Board newBoard = Board();
                            for (int i = 0; i < 36; i++)
                                newBoard.cells[i] = boards[step].cells[i];
                            newBoard.player = boards[step].player % 2 + 1;
                            boards.push_back(newBoard);
                            step++;
                        }
                    } else {
                        cout << "Pole jest zajete" << endl;
                    }
                } else {
                    cout << "Niepoprawny ruch" << endl;
                    cout << "Wpisz \'h\' by wyswietlic pomoc." << endl;
                }
            } else {
                cout << "Niepoprawne dane wejsciowe" << endl;
                cout << "Wpisz \'h\' by wyswietlic pomoc." << endl;
            }
        }
        // po zakończeniu gry przechodzi do przeglądania rozgrywki
        browse();
    }

    /// Zwraca token gracza
    char getToken(short player) {
        if (player == 1)
            return token1;
        else if (player == 2)
            return token2;
        else
            return ' ';
    }

    /// Wyświetla podaną plansze i informacje gracza
    void printBoard(Board board) {
        printf(TOPLINE);
        printf("\n");

        char hTab[6];
        for (int line = 0; line < 6; line++) {
            for (int i = 0; i < 6; i++)
                hTab[i] = getToken(board.getTokenId(i + (line * 6)));
            printf(VARIABLELINE, hTab[0], hTab[1], hTab[2], hTab[3], hTab[4],
                   hTab[5]);
            if (line == 0) {
                if (board.player == 1)
                    printf("Ruch gracza pierwszego\n");
                else if (board.player == 2)
                    printf("Ruch gracza drugiego\n");
            } else if (line == 1) {
                if (board.player == 1)
                    cout << "Imie:\t" << name1 << "\n";
                else if (board.player == 2)
                    cout << "Imie:\t" << name2 << "\n";
            } else if (line == 2) {
                if (board.player == 1)
                    cout << "Token:\t" << token1 << "\n";
                else if (board.player == 2)
                    cout << "Token:\t" << token2 << "\n";
            } else if (line == 3) {
                if (board.player == 1)
                    cout << "Czas:\t" << time1 << "\n";
                else if (board.player == 2)
                    cout << "Czas:\t" << time2 << "\n";
            } else {
                printf("\n");
            }
            if (line != 5) printf(MIDLINE), printf("\n");
        }
        printf(BOTTOMLINE);
        printf("\n");
    }

    /// Wczytuje podany w instrukcji stan gry
    void loadPredefinedState() {
        // Tablice pól zajętych przez graczy
        int t1[] = {4, 6, 7, 9, 13, 18, 19, 26};
        int t2[] = {1, 8, 10, 24, 28, 31, 34, 35};
        Board newBoard = Board();
        for (int i = 0; i < 8; i++) {
            newBoard.placeToken(t1[i], 1);
            newBoard.placeToken(t2[i], 2);
        }
        newBoard.player = boards[step].player % 2 + 1;
        boards.push_back(newBoard);
        step++;
    }

   public:
    /// Wczytuje imiona graczy
    void scanNames() {
        while (name1.empty()) {
            cout << "Podaj imie pierwszego gracza: ";
            getline(cin, name1);
        }
        while (name2.empty()) {
            cout << "Podaj imie drugiego gracza: ";
            getline(cin, name2);
            // Imiona nie mogą być takie same
            if (name1 == name2) {
                cout << "Imiona nie moga byc takie same." << endl;
                name2 = "";
            }
        }
    }

    /// Wczytuje żetony graczy
    void scanTokens() {
        string tempToken;

        while (token1 == '\0') {
            cout << "Podaj zeton dla pierwzego gracza (" << name1 << "): ";
            getline(cin, tempToken);

            // Zeton nie może byc dłuższy niż jeden znak
            if (tempToken.length() > 1) {
                cout << "Zeton moze byc tylko jednym znakiem." << endl;
            } else {
                token1 = tempToken[0];
            }
        }
        while (token2 == '\0') {
            cout << "Podaj zeton dla drugiego gracza (" << name2 << "): ";
            getline(cin, tempToken);

            // Zeton nie może byc dłuższy niż jeden znak
            if (tempToken.length() > 1) {
                cout << "Zeton moze byc tylko jednym znakiem." << endl;
            } else {
                token2 = tempToken[0];
                // Zetony nie mogą być takie same
                if (token1 == token2) {
                    cout << "Zetony nie moga byc takie same" << endl;
                    token2 = '\0';
                }
            }
        }
    }

    /// Wczytuje dostępny dla graczy czas
    void scanTime() {
        string inp;
        while (time1 == 0) {
            cout << "Podaj czas w sekundach dla pierwzego gracza: ";
            getline(cin, inp);
            time1 = atoi(inp.c_str());
            // Wartość bezwzgledna z podanej liczby
            if (time1 < 0) time1 *= -1;
        }
        while (time2 == 0) {
            cout << "Podaj czas w sekundach dla drugiego gracza: ";
            getline(cin, inp);
            time2 = atoi(inp.c_str());
            // Wartość bezwzgledna z podanej liczby
            if (time2 < 0) time2 *= -1;
        }
    }

    /// Otwiera menu gry
    void openMenu() {
        system("cls");
        printf("\n\t\tPENTAGO\n\n");
        printf("\tMENU:\n\n");
        printf("\t1. Kolko i krzyzyk\n");
        printf("\t2. Pentago\n");
        printf("\t3. Zamknij program\n");
        while (true) {
            string inp;
            getline(cin, inp);
            system("cls");
            if (inp == "1") {
                pentago = false;
                play();
            } else if (inp == "2")
                play();
            else if (inp == "3")
                exit(0);
            else
                openMenu();
        }
    }
};

int main() {
    Game game = Game();
    game.scanNames();
    game.scanTokens();
    game.scanTime();
    game.openMenu();

    return 0;
}
