#include "stdafx.h"
#include "SimulatedAnnealing.h"
#include <algorithm>
#include <iostream>
#include <Windows.h>

using namespace std;

const double e = 2.71828182845904523536;

int SimulatedAnnealing::algorithm() {
    int* minimalTour;
    int* nextState = new int[size];
    int* currentPermutation = new int[size];

    if (autoGenerateInitialTemperature) {
        initialTemperature = generateInitialTemperature();
    }
    double T = initialTemperature;

    currentPermutation = randomPermutation();								// wygenerowanie losowej permutacji
    int min = cost(currentPermutation);										// policzenie kosztu pierwszej permutacji
    int currentPermutationCost = min;
    minimalTour = setMinimalTour(currentPermutation);

    int a, b;

    while (T > finalTemperature) {
        for (int i = 0; i < period; i++) {									// epoki
            do {
                a = rand() % size;
                b = rand() % size;
            } while (a == b);												// losowanie dwóch miast
            swap(currentPermutation[a], currentPermutation[b]);				// zamiana miast
            int nextStateCost = cost(currentPermutation);					// nextState
            if (nextStateCost < currentPermutationCost) {
                currentPermutationCost = nextStateCost;
                if (nextStateCost < min) {
                    min = nextStateCost;
                    minimalTour = setMinimalTour(currentPermutation);
                }
            } else if (!probability(nextStateCost - currentPermutationCost, T)) {
                swap(currentPermutation[a], currentPermutation[b]);
            } else currentPermutationCost = nextStateCost;
        }
        T *= alpha;
    }
    delete[]currentPermutation;
    delete[]minimalTour;
    delete[]nextState;
    return min;
}

void SimulatedAnnealing::showPermutation(const int * permutation) {
    for (int i = 0; i < size; i++) {
        cout << permutation[i] << "  ";
    }
    cout << endl;
}

bool SimulatedAnnealing::probability(int delta, double Temperature) {
    double prob = pow(e, -delta / Temperature);
    double r = (double)rand() / RAND_MAX;
    return r < prob;
}

double SimulatedAnnealing::generateInitialTemperature() {
    double delta = 0;
    int *currentPermutation = randomPermutation();							//losowa permutacja
    int currentPermutationCost = cost(currentPermutation);
    int *nextPermutation;

    for (int i = 0; i < size*size; i++) {
        nextPermutation = randomPermutation();
        double delta2 = cost(nextPermutation) - currentPermutationCost;
        delete[]nextPermutation;
        if (delta2>delta) delta = delta2;
    }
    delete[]currentPermutation;
    return delta * 10;
}

int*  SimulatedAnnealing::randomPermutation() {
    int * permutation = new int[size];
    for (int i = 0; i < size; i++) {
        permutation[i] = i;
    }
    random_shuffle(&permutation[0], &permutation[size]);
    return permutation;
}

int* SimulatedAnnealing::setMinimalTour(const int * permutation) {
    int * next = new int[size];
    for (int i = 0; i < size; i++) {
        next[i] = permutation[i];
    }
    return next;
}

void SimulatedAnnealing::swapPermutation(const int * permutation, int* nextState) {
    //int * next = new int[size];
    for (int i = 0; i < size; i++) {
        nextState[i] = permutation[i];
    }

    int a, b;
    do {
        a = rand() % size;
        b = rand() % size;
    } while (a == b);

    swap(nextState[a], nextState[b]);
    //	return next;
}

int SimulatedAnnealing::cost(int* permutation) {
    return matrix->costPermutation(permutation);
}

SimulatedAnnealing::SimulatedAnnealing(string filename) {
    matrix = new Matrix(filename);
    this->size = matrix->getSize();

    //Default values of prameters
    period = 50;
    alpha = 0.999;
    initialTemperature = 100;
    finalTemperature = 0.2;
    autoGenerateInitialTemperature = true;
}

SimulatedAnnealing::~SimulatedAnnealing() {
    delete matrix;
    size = 0;
}