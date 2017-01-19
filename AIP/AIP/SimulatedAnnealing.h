#pragma once
#include "Matrix.h"
#include <string>

using namespace std;

class SimulatedAnnealing {
private:
    Matrix * matrix;
    int size;
    int* bestPath;

    double generateInitialTemperature();
    bool acceptWorseSolution(int delta, double temperature);
    int* getRandomPermutation(bool blackAndWhite);
    int* copyPermutation(const int* permutation);
    int computeCostOf(int* permutation);
    void showPermutation(const int*);
    void setTwoRandomCities(int & cityA, int & cityB, bool blackAndWhite);

public:
    int period;
    double alpha;
    double initialTemperature;
    double finalTemperature;
    bool autoGenerateInitialTemperature;
    // second variant - cities are divided into two sets (odd and even), path can only exist if cities are alternately even and odd i.e. 0-1-2-3-4 is correct and 2-4-1-3-0 is incorrect
    bool blackAndWhite;

    int algorithm();
    string bestPathToString();

    int getSize() {
        return size;
    };

    SimulatedAnnealing(string filename);

    ~SimulatedAnnealing();
};

