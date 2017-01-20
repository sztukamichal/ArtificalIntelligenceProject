#pragma once
#include "Matrix.h"
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <sstream>

using namespace std;

class GeneticAlgorithm {

private:
    Matrix* matrix;
    vector<int*>* population;
    float costOfAllPopulation;							// suma wartosci funkcji celu dla ca³ej populacji

    int size;                                       // ilosc miast
    int *bestPath;                                  // najlepsze rozwiazanie do tej pory
    int minCost;                                    // minimalny koszt podrozy
    bool solved;                                    // parametr okreslajacy czy problem zostal rozwiazany

    void calculateAllCosts(vector<int*>* population);
    int* cross(int*, int*);
    void setParameters(int sizeOfPopulation, int numberOfPopulation, int numberOfGenes, int probability, int numberOfChild);
    void initPopulation(int);
    void sortPopulation();
    void deleteRunt(int);
    void showPopulation(vector<int*>* population);
    void showPermutation(const int*);
    bool isInPermutation(const int* permutation, int city);
    int* copyPermutation(const int* permutation);
    int* getRandomPermutation();
    vector<int*>*chooseParents();
    void createNewPopulation();
    void mutation(int * child);
    bool hasDuplicate(int*, vector<int *>* population);
    void setTwoRandomCities(int & cityA, int & cityB);

public:
    //PARAMETRYY ALGORYTMU
    int sizeOfPopulation;					// rozmiar populacji
    int numberOfPopulations;					// Liczba populacji
    int numberOfGenes;						// liczba genów ulegajaca mutacji
    int probability;						// prawdopodobienstwo mutacji
    int numberOfChildren; 						// Ilosc potomkow
    // second variant - cities are divided into two sets (odd and even), path can only exist if cities are alternately even and odd i.e. 0-1-2-3-4 is correct and 2-4-1-3-0 is incorrect
    bool blackAndWhite;

    int algorithm();
    int getSize() {
        return size;
    };
    void showMatrix() {
        matrix->show_matrix();
    };
    string bestPathToString();
    bool operator() (int* & a, int* & b) const {
        return (matrix->costPermutation(a) < matrix->costPermutation(b));
    }

    GeneticAlgorithm(std::string);

    ~GeneticAlgorithm();
};