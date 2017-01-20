#pragma once
#include "Matrix.h"
#include "TabuList.h"
#include <time.h>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

class TabuSearch {

private:
    // DANE ORAZ SZUKANE PROBLEMU
    Matrix * matrix;				// macierz
    TabuList *tabu;					// lista tabu
    int size;						// ilosc miast
    int *bestPath;					// najlepsza permutacja
    int minCost;					// minimalny koszt podrozy
    bool solved;					// parametr okreslajacy czy problem zostal rozwiazany

    int* getRandomPermutation();
    void setTwoRandomCities(int & cityA, int & cityB);
    int* chooseNeighbour(int, const int*, int*, int*, int*);
    int* diversification();
    void setMinimalPath(int*);
public:
    int algorithm();

    //PARAMETRY ALGORYTMU
    int iterations;					// liczba iteracji petli glownej algorytmu
    int not_change;					// maksymalna liczba iteracji bez poprawy rozwiazania jako warunek koncowy
    int div_not_change;				// maksymalna liczba iteracji bez poprawy rozwiazania dla stosowania dywersyfikacji
    double alg_time;				// czas dzialania algorytmu
    int num_of_candidates;			// liczba kandydatów
    int tabu_length;				// dlugosc listy tabu
    bool diverisfication_is_On;		// 0 - nie stosujemy dywersyfikacji, 1 - stosujemy dywersyfikacje
    int stop_condition;				// 0 - iteracje , 1 - czas , 2 - not change
    bool blackAndWhite;

    void showPermutation(const int*);
    void showMatrix() {
        matrix->show_matrix();
    };
    string bestPathToString();

    Matrix* getMatrix() {
        return matrix;
    };
    int getSize() {
        return size;
    };
    void setParameters(int i, int n, int divn, double t, int k, int t_length, bool div, int stop);
    TabuSearch(string filename);

    ~TabuSearch(void);
};

