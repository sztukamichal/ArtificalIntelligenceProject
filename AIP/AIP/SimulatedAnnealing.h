#pragma once
#include "Matrix.h"
#include <string>

class SimulatedAnnealing {
private:
    Matrix * matrix;
    int size;

    double generateInitialTemperature();
    bool probability(int delta, double temperature);
    int* randomPermutation();
    void swapPermutation(const int * permutation, int* nextState);
    int* setMinimalTour(const int* permutation);
    int cost(int* permutation);
    void showPermutation(const int*);

public:
    int period;
    double alpha;
    double initialTemperature;
    double finalTemperature;
    bool autoGenerateInitialTemperature;

    int algorithm();
    Matrix* getMatrix() {
        return matrix;
    }
    int getSize() {
        return size;
    };

    SimulatedAnnealing(std::string filename);

    ~SimulatedAnnealing();
};

