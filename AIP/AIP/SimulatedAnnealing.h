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
    int* getRandomPermutation();
    int* copyPermutation(const int* permutation);
    int computeCostOf(int* permutation);
    void showPermutation(const int*);

public:
    int period;
    double alpha;
    double initialTemperature;
    double finalTemperature;
    bool autoGenerateInitialTemperature;

    int algorithm();
    string bestPathToString();

    int getSize() {
        return size;
    };

    SimulatedAnnealing(string filename);

    ~SimulatedAnnealing();
};

