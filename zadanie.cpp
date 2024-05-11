#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

void wypelnianie(int tablica[], int rozmiar, char jakieLiczby) {
    srand(time(NULL));

    if (jakieLiczby == 'c' || jakieLiczby == 'C') {
        for (int i = 0; i < rozmiar; i++) {
            tablica[i] = rand();
        }
    } else if (jakieLiczby == 'r' || jakieLiczby == 'R') {
        for (int i = 0; i < rozmiar; i++) {
            tablica[i] = rand() / static_cast<double>(RAND_MAX + 1);
        }
    }
}

void zapiszWynikiDoPliku(string nazwaPliku, vector<string> nazwySortowan, vector<vector<double>> czasySortowan) {
    ofstream plik(nazwaPliku);
    if (plik.is_open()) {
        plik << "Nazwa sortowania\tRozmiar tablicy\tCzas (ms)" << endl;
        for (size_t i = 0; i < nazwySortowan.size(); ++i) {
            for (size_t j = 0; j < czasySortowan[i].size(); ++j) {
                plik << nazwySortowan[i] << "\t" << (j + 1) << "\t" << czasySortowan[i][j] << endl;
            }
        }
        cout << "Wyniki zostaly zapisane do pliku: " << nazwaPliku << endl;
    } else {
        cout << "Blad podczas zapisu do pliku!" << endl;
    }
    plik.close();
}

double pomiarCzasuSortowania(void (*sortowanie)(int[], int), int tablica[], int rozmiar) {
    LARGE_INTEGER start, end, freq;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
    sortowanie(tablica, rozmiar);
    QueryPerformanceCounter(&end);
    return ((end.QuadPart - start.QuadPart) * 1000.0) / freq.QuadPart; // Czas w milisekundach
}

bool czyPosortowana(int tablica[], int rozmiar) {
    for (int i = 0; i < rozmiar - 1; i++) {
        if (tablica[i] > tablica[i + 1]) {
            return false; // Tablica nie jest posortowana
        }
    }
    return true; // Tablica jest posortowana
}

void sortowanieBabelkowe(int arr[], int n) {
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}
void sortowaniePrzezWybor(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }
        if (minIndex != i) {
            int temp = arr[i];
            arr[i] = arr[minIndex];
            arr[minIndex] = temp;
        }
    }
}
void sortowaniePrzezWstawianie(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}
//scalanie


void sortowaniePrzezZliczanie(int arr[], int n) {
    int max = *max_element(arr, arr + n);
    int min = *min_element(arr, arr + n);
    int range = max - min + 1;

    vector<int> count(range), output(n);
    for (int i = 0; i < n; i++)
        count[arr[i] - min]++;

    for (int i = 1; i < range; i++)
        count[i] += count[i - 1];

    for (int i = n - 1; i >= 0; i--) {
        output[count[arr[i] - min] - 1] = arr[i];
        count[arr[i] - min]--;
    }

    for (int i = 0; i < n; i++)
        arr[i] = output[i];
}

void heapify(int arr[], int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest])
        largest = left;

    if (right < n && arr[right] > arr[largest])
        largest = right;

    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void sortowaniePrzezKopcowanie(int arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

void sortowanieKubelkowe(int arr[], int n) {
    int max = *max_element(arr, arr + n);
    int min = *min_element(arr, arr + n);
    int range = max - min + 1;

    vector<vector<int>> bucket(range);

    for (int i = 0; i < n; i++) {
        int index = (arr[i] - min) * range / (max - min + 1);
        bucket[index].push_back(arr[i]);
    }

    for (int i = 0; i < range; i++)
        sort(bucket[i].begin(), bucket[i].end());

    int index = 0;
    for (int i = 0; i < range; i++)
        for (int j = 0; j < bucket[i].size(); j++)
            arr[index++] = bucket[i][j];
}

int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return (i + 1);
}

void sortowanieSzybkieHelper(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        sortowanieSzybkieHelper(arr, low, pi - 1);
        sortowanieSzybkieHelper(arr, pi + 1, high);
    }
}

void sortowanieSzybkie(int arr[], int n) {
    sortowanieSzybkieHelper(arr, 0, n - 1);
}

void wyswietlTablice(int tablica[], int rozmiar) {
    cout << "Zawartosc tablicy:" << endl;
    for (int i = 0; i < rozmiar; i++) {
        cout << tablica[i] << " ";
    }
    cout << endl;
}

int main() {
    int flaga = 0;
    int flagaWypelniania = 0;
    int rozmiarTablicy = 0;
    char jakieLiczby = ' ';
    int* mojaTablica = NULL;

    do {
        cout << "+-------------------------------+" << endl;
        cout << "|--PROGRAM-Z-SUPER-SORTOWANIEM--|" << endl;
        cout << "+-------------------------------+" << endl;
        cout << "|1. Wypelnianie Tablicy         |" << endl;
        cout << "+-------------------------------+" << endl;
        cout << "|2. Sortowanie babelkowe        |" << endl;
        cout << "+-------------------------------+" << endl;
        cout << "|3. Sortowanie przez wybor      |" << endl;
        cout << "+-------------------------------+" << endl;
        cout << "|4. Sortowanie przez wstawianie |" << endl;
        cout << "+-------------------------------+" << endl;
        cout << "|5. Sortowanie przez scalanie   |" << endl;
        cout << "+-------------------------------+" << endl;
        cout << "|6. Sortowanie przez zliczanie  |" << endl;
        cout << "+-------------------------------+" << endl;
        cout << "|7. Sortowanie przez kopcowanie |" << endl;
        cout << "+-------------------------------+" << endl;
        cout << "|8. Sortowanie kubelkowe        |" << endl;
        cout << "+-------------------------------+" << endl;
        cout << "|9. Sortowanie szybkie          |" << endl;
        cout << "+-------------------------------+" << endl;
        cout << "|10. Wszystkie na raz!!!        |" << endl;
        cout << "+-------------------------------+" << endl;
        cout << "|11. Wyswietl tablice           |" << endl;
        cout << "+-------------------------------+" << endl;
        cout << "|12. Wyjscie                    |" << endl;
        cout << "+-------------------------------+" << endl;
        cout << "Wybierz opcje: ";
        int wyborOpcjiMenu;
        cin >> wyborOpcjiMenu;

        switch(wyborOpcjiMenu) {
            case 1:
                system("CLS");
                if (mojaTablica != NULL) {
                    system("CLS");
                    cout << "Tablica zostala juz wypelniona!" << endl;
                    Sleep(1500);
                    break;
                }

                int tempRozmiar;
                char tempLiczba;

                system("CLS");
                cout << "+--------------------------------+" << endl;
                cout << "|----WYBIERZ-WIELKOSC-TABLICY----|" << endl;
                cout << "+--------------------------------+" << endl;
                cout << "|1. 30.000                       |" << endl;
                cout << "+--------------------------------+" << endl;
                cout << "|2. 50.000                       |" << endl;
                cout << "+--------------------------------+" << endl;
                cout << "|3. 100.000                      |" << endl;
                cout << "+--------------------------------+" << endl;
                cout << "|4. 150.000                      |" << endl;
                cout << "+--------------------------------+" << endl;
                cout << "|5. 200.000                      |" << endl;
                cout << "+--------------------------------+" << endl;
                cout << "|6. 500.000                      |" << endl;
                cout << "+--------------------------------+" << endl;
                cout << "|7. 1.000.000                    |" << endl;
                cout << "+--------------------------------+" << endl;
                cout << "|8. 2.000.000                    |" << endl;
                cout << "+--------------------------------+" << endl;
                cout << "|9. 5.000.000                    |" << endl;
                cout << "+--------------------------------+" << endl;
                cout << "|10. 10.000.000                  |" << endl;
                cout << "+--------------------------------+" << endl;
                cout << "Wybierz: ";
                cin >> tempRozmiar;

                if (tempRozmiar < 1 || tempRozmiar > 10) {
                    system("CLS");
                    cout << "Wprowadziles zla wartosc!" << endl;
                    Sleep(1500);
                    break;
                }

                system("CLS");
                cout << "+---------------------------------+" << endl;
                cout << "|---WYBIERZ-JAKIE-CHCESZ-LICZBY---|" << endl;
                cout << "+---------------------------------+" << endl;
                cout << "|C = CALKOWITE                    |" << endl;
                cout << "|R = RZECZYWISTE                  |" << endl;
                cout << "+---------------------------------+" << endl;
                cout << "Wybierz: ";
                cin >> tempLiczba;

                if (tempLiczba != 'c' && tempLiczba != 'C' && tempLiczba != 'r' && tempLiczba != 'R') {
                    system("CLS");
                    cout << "Wprowadziles zla wartosc!" << endl;
                    Sleep(1500);
                    break;
                }

                jakieLiczby = tempLiczba;

                if(tempRozmiar == 1) rozmiarTablicy = 30000;
                else if(tempRozmiar == 2) rozmiarTablicy = 50000;
                else if(tempRozmiar == 3) rozmiarTablicy = 100000;
                else if(tempRozmiar == 4) rozmiarTablicy = 150000;
                else if(tempRozmiar == 5) rozmiarTablicy = 200000;
                else if(tempRozmiar == 6) rozmiarTablicy = 500000;
                else if(tempRozmiar == 7) rozmiarTablicy = 1000000;
                else if(tempRozmiar == 8) rozmiarTablicy = 2000000;
                else if(tempRozmiar == 9) rozmiarTablicy = 5000000;
                else if(tempRozmiar == 10) rozmiarTablicy = 10000000;

                system("CLS");

                mojaTablica = new int[rozmiarTablicy];



                wypelnianie(mojaTablica, rozmiarTablicy, jakieLiczby);

                cout << "Tablica zostala wypelniona!" << endl;

                Sleep(1500);
                system("CLS");
                break;

            case 2:
                if (czyPosortowana(mojaTablica, rozmiarTablicy)) {
                    cout << "Tablica jest juz posortowana!" << endl;
                } else {
                    sortowanieBabelkowe(mojaTablica, rozmiarTablicy);
                    cout << "Sortowanie babelkowe zostalo wykonane!" << endl;
                }
                break;
            case 3:
                if (czyPosortowana(mojaTablica, rozmiarTablicy)) {
                    cout << "Tablica jest juz posortowana!" << endl;
                } else {
                    sortowaniePrzezWybor(mojaTablica, rozmiarTablicy);
                    cout << "Sortowanie przez wybor zostalo wykonane!" << endl;
                }
                break;
            case 4:
                if (czyPosortowana(mojaTablica, rozmiarTablicy)) {
                    cout << "Tablica jest juz posortowana!" << endl;
                } else {
                    sortowaniePrzezWstawianie(mojaTablica, rozmiarTablicy);
                    cout << "Sortowanie przez wstawianie zostalo wykonane!" << endl;
                }
                break;
            case 5:
                //if (mojaTablica == NULL) {
                //    cout << "Tablica nie zostala jeszcze wypelniona!" << endl;
                //    Sleep(1500);
                //    break;
                //}
                //sortowaniePrzezScalanie(mojaTablica, rozmiarTablicy);
                //cout << "Sortowanie przez scalanie zostalo wykonane!" << endl;
                break;
            case 6:
                if (mojaTablica == NULL) {
                    cout << "Tablica nie zostala jeszcze wypelniona!" << endl;
                    Sleep(1500);
                    break;
                }
                sortowaniePrzezZliczanie(mojaTablica, rozmiarTablicy);
                cout << "Sortowanie przez zliczanie zostalo wykonane!" << endl;
                break;
            case 7:
                if (mojaTablica == NULL) {
                    cout << "Tablica nie zostala jeszcze wypelniona!" << endl;
                    Sleep(1500);
                    break;
                }
                sortowaniePrzezKopcowanie(mojaTablica, rozmiarTablicy);
                cout << "Sortowanie przez kopcowanie zostalo wykonane!" << endl;
                break;
            case 8:
                if (mojaTablica == NULL) {
                    cout << "Tablica nie zostala jeszcze wypelniona!" << endl;
                    Sleep(1500);
                    break;
                }
                sortowanieKubelkowe(mojaTablica, rozmiarTablicy);
                cout << "Sortowanie kubelkowe zostalo wykonane!" << endl;
                break;
            case 9:
                if (mojaTablica == NULL) {
                    cout << "Tablica nie zostala jeszcze wypelniona!" << endl;
                    Sleep(1500);
                    break;
                }
                sortowanieSzybkie(mojaTablica, rozmiarTablicy);
                cout << "Sortowanie szybkie zostalo wykonane!" << endl;
                break;
            case 10:
                {
                    if (mojaTablica == NULL) {
                        cout << "Tablica nie zostala jeszcze wypelniona!" << endl;
                        Sleep(1500);
                        break;
                    }
                    vector<string> nazwySortowan = {"Sortowanie babelkowe", "Sortowanie przez wybor", "Sortowanie przez wstawianie", "Sortowanie przez scalanie", "Sortowanie przez zliczanie", "Sortowanie przez kopcowanie", "Sortowanie kubelkowe", "Sortowanie szybkie"};
                    vector<void (*)(int[], int)> funkcjeSortowania = {sortowanieBabelkowe, sortowaniePrzezWybor, sortowaniePrzezWstawianie, sortowaniePrzezZliczanie, sortowaniePrzezKopcowanie, sortowanieKubelkowe, sortowanieSzybkie};
                    vector<vector<double>> czasySortowan(funkcjeSortowania.size(), vector<double>());

                    for (size_t i = 0; i < funkcjeSortowania.size(); ++i) {
                        czasySortowan[i].push_back(pomiarCzasuSortowania(funkcjeSortowania[i], mojaTablica, rozmiarTablicy));
                    }

                    zapiszWynikiDoPliku("wyniki.txt", nazwySortowan, czasySortowan);
                    break;
                }
            case 11:
                if (mojaTablica == NULL) {
                    cout << "Tablica nie zostala jeszcze wypelniona!" << endl;
                    Sleep(1500);
                    break;
                }
                wyswietlTablice(mojaTablica, rozmiarTablicy);
                break;
            case 12:
                system("CLS");
                cout << "+-------------------------------+" << endl;
                cout << "|         Do zobaczenia!        |" << endl;
                cout << "+-------------------------------+" << endl;
                flaga = 1;
                break;
            default:
                cout << "+-------------------------------+" << endl;
                cout << "|Nie ma takiej opcji w menu!!!  |" << endl;
                cout << "+-------------------------------+" << endl;
                break;
        }

    } while (flaga != 1);

    delete[] mojaTablica;

    return 0;
}
