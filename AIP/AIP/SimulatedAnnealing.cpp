#include "stdafx.h"
#include "SimulatedAnnealing.h"
#include <algorithm>
#include <iostream>
#include <Windows.h>

using namespace std;

const double e = 2.71828182845904523536;

int SimulatedAnnealing::algorithm() {
    int* currentPermutation = new int[size];
    currentPermutation = getRandomPermutation();

    if (autoGenerateInitialTemperature) {
        initialTemperature = generateInitialTemperature();
    }
    double temperature = initialTemperature;

    int currentPermutationCost = computeCostOf(currentPermutation);
    int nextPermutationCost;
    bestPath = copyPermutation(currentPermutation);
    int minimalFoundCost = currentPermutationCost;

    int cityA, cityB;
    bool isBetterSolution;
    bool isTheBestSoFar;

    while (temperature > finalTemperature) {
        for (int i = 0; i < period; i++) {
            do {
                cityA = rand() % size;
                cityB = rand() % size;
            } while (cityA == cityB);
            swap(currentPermutation[cityA], currentPermutation[cityB]);

            nextPermutationCost = computeCostOf(currentPermutation);
            isBetterSolution = nextPermutationCost < currentPermutationCost;

            if (isBetterSolution) {
                currentPermutationCost = nextPermutationCost;
                isTheBestSoFar = nextPermutationCost < minimalFoundCost;
                if (isTheBestSoFar) {
                    minimalFoundCost = nextPermutationCost;
                    delete[]bestPath;
                    bestPath = copyPermutation(currentPermutation);
                }
            } else if (!acceptWorseSolution(nextPermutationCost - currentPermutationCost, temperature)) {
                swap(currentPermutation[cityA], currentPermutation[cityB]);
            } else
                currentPermutationCost = nextPermutationCost;
        }
        temperature *= alpha;
    }
    delete[]currentPermutation;
    return minimalFoundCost;
}

bool SimulatedAnnealing::acceptWorseSolution(int delta, double Temperature) {
    double prob = pow(e, -delta / Temperature);
    double r = (double)rand() / RAND_MAX;
    return r < prob;
}

double SimulatedAnnealing::generateInitialTemperature() {
    double delta = 0, delta2;
    int *currentPermutation = getRandomPermutation();							//losowa permutacja
    int currentPermutationCost = computeCostOf(currentPermutation);
    int *nextPermutation;

    for (int i = 0; i < size*size; i++) {
        nextPermutation = getRandomPermutation();
        delta2 = computeCostOf(nextPermutation) - currentPermutationCost;
        delete[]nextPermutation;
        if (delta2>delta)
            delta = delta2;
    }
    delete[]currentPermutation;
    return delta * 10;
}

int*  SimulatedAnnealing::getRandomPermutation() {
    int * permutation = new int[size];
    for (int i = 0; i < size; i++) {
        permutation[i] = i;
    }
    random_shuffle(&permutation[0], &permutation[size]);
    return permutation;
}

int* SimulatedAnnealing::copyPermutation(const int * permutation) {
    int * next = new int[size];
    for (int i = 0; i < size; i++) {
        next[i] = permutation[i];
    }
    return next;
}

int SimulatedAnnealing::computeCostOf(int* permutation) {
    return matrix->costPermutation(permutation);
}

string SimulatedAnnealing::bestPathToString() {
    string result = "";
    for (int i = 0; i < size; i++) {
        if (i < size - 1)
            result += bestPath[i] + "  -> ";
        else
            result += bestPath[i];
    }
    return result;
};

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
    delete[]bestPath;
    size = 0;
}