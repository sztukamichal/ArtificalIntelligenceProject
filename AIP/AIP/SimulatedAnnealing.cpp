#include "stdafx.h"
#include "SimulatedAnnealing.h"
#include <algorithm>
#include <iostream>
#include <Windows.h>
#include <sstream>

using namespace std;

const double e = 2.71828182845904523536;

int SimulatedAnnealing::algorithm() {
    int* currentPermutation = new int[size];
    currentPermutation = getRandomPermutation(blackAndWhite);

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
            setTwoRandomCities(cityA, cityB, blackAndWhite);
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
    int *currentPermutation = getRandomPermutation(blackAndWhite);							//losowa permutacja
    int currentPermutationCost = computeCostOf(currentPermutation);
    int *nextPermutation;

    for (int i = 0; i < size*size; i++) {
        nextPermutation = getRandomPermutation(blackAndWhite);
        delta2 = computeCostOf(nextPermutation) - currentPermutationCost;
        delete[]nextPermutation;
        if (delta2>delta)
            delta = delta2;
    }
    delete[]currentPermutation;
    return delta * 10;
}

int*  SimulatedAnnealing::getRandomPermutation(bool blackAndWhite) {
    int * permutation = new int[size];
    if (blackAndWhite) {
        int numOfOdds = 0;
        int numOfEven = 0;
        if (size % 2 == 0) {
            numOfOdds = size / 2;
            numOfEven = size / 2;
        } else {
            numOfOdds = (size - 1) / 2;
            numOfEven = (size + 1) / 2;
        }
        int * oddSet = new int[numOfOdds];
        int * evenSet = new int[numOfEven];

        for (int i = 0; i < size; i++) {
            if (i % 2 == 0) {
                evenSet[i / 2] = i ;
            } else
                oddSet[(i-1) / 2] = i;
        }
        random_shuffle(&oddSet[0], &oddSet[numOfOdds]);
        random_shuffle(&evenSet[0], &evenSet[numOfEven]);
        for (int i = 0; i < size; i++) {
            if (i % 2 == 0) {
                permutation[i] = evenSet[i / 2];
            } else {
                permutation[i] = oddSet[(i - 1) / 2];
            }
        }

    } else {
        for (int i = 0; i < size; i++) {
            permutation[i] = i;
        }
        random_shuffle(&permutation[0], &permutation[size]);
    }
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
    ostringstream resultStream;
    int j = 0;
    int i = 0;
    for (j = 0; j < size; j++) {
        if (bestPath[j] == 0) {
            break;
        }
    }
    for (j; i < size; i++, j++) {
        if (j == size) {
            j = 0;
        }
        if (i < size - 1) {
            resultStream << bestPath[j];
            resultStream << " -> ";
        } else
            resultStream << bestPath[j];
    }
    return resultStream.str();

};

void SimulatedAnnealing::setTwoRandomCities(int & cityA, int & cityB, bool blackAndWhite) {
    if (blackAndWhite) {
        do {
            cityA = rand() % size;
            cityB = rand() % size;
        } while (cityA == cityB || (cityA % 2 == 0 && cityB % 2 != 0) || (cityA % 2 == 1 && cityB % 2 != 1));
    } else {
        do {
            cityA = rand() % size;
            cityB = rand() % size;
        } while (cityA == cityB);
    }
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
    blackAndWhite = false;
}

SimulatedAnnealing::~SimulatedAnnealing() {
    delete matrix;
    delete[]bestPath;
    size = 0;
}