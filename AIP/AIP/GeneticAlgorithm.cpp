#include "stdafx.h"
#include "GeneticAlgorithm.h"

using namespace std;

GeneticAlgorithm::GeneticAlgorithm(string filename) {
    matrix = new Matrix(filename);
    size = matrix->getSize();
    bestPath = new int[size];
    for (int i = 0; i<size; i++) {
        bestPath[i] = 0;
    }
    minCost = 0;
    solved = false;
    costOfAllPopulation = 0;

    //Default values of prameters
    sizeOfPopulation = 50;
    numberOfPopulations = 300;
    numberOfGenes = 2;
    probability = 50;
    numberOfChildren = 50;
}

void GeneticAlgorithm::showPopulation(vector<int*>* population) {
    vector<int*>::iterator it;
    int i = 0;
    cout << "\n";
    for (it = population->begin(); it != population->end(); ++it) {// if (i<10){
        cout << "OSOBNIK: " << i;
        showPermutation(*it);
        cout << "COST " << matrix->costPermutation(*it) << endl;//}
        i++;
    }
}

int* GeneticAlgorithm::cross(int* permutationA, int* permutationB) {
    int* child = new int[size]();
    for (int i = 0; i < size; i++) {
        child[i] = -1;
    }
    // Get start and end sub tour positions for parent1's tour
    int startPos = rand() % size;
    int endPos = rand() % size;

    // Loop and add the sub tour from parent1 to our child
    for (int i = 0; i < size; i++) {
        // If our start position is less than the end position
        if (startPos < endPos && i > startPos && i < endPos) {
            child[i] = permutationA[i];
        } // If our start position is larger
        else if (startPos > endPos) {
            if (!(i < startPos && i > endPos)) {
                child[i] = permutationA[i];
            }
        }
    }
    // Loop through parent2's city tour
    for (int i = 0; i < size; i++) {
        // If child doesn't have the city add it
        if (!isInPermutation(child, permutationB[i])) {
            // Loop to find a spare position in the child's tour
            for (int ii = 0; ii < size; ii++) {
                // Spare position found, add city
                if (child[ii] == -1) {
                    child[ii] = permutationB[i];
                    break;
                }
            }
        }
    }
    return child;
}

void GeneticAlgorithm::showPermutation(const int* permutation) {
    cout << endl;
    for (int i = 0; i < size; i++) {
        cout << permutation[i] << "  ";
    }
}

bool GeneticAlgorithm::isInPermutation(const int* permutation, int city) {
    for (int i = 0; i < this->size; i++) {
        if (permutation[i] == city)
            return true;
    }
    return false;
}

bool GeneticAlgorithm::hasDuplicate(int* permutation, vector<int *>* population) {
    vector<int*>::iterator it;
    if (population->size() == 0) {
        return false;
    }
    for (it = population->begin(); it != population->end(); ++it) {
        int count = 0;
        for (int i = 0; i < size; i++) {
            if ((*it)[i] != permutation[i]) {
                break;
            }
            count++;
        }
        if (count == size) {
            return true;
        }
    }
    return false;
}

int GeneticAlgorithm::algorithm() {

    population = new vector<int*>();
    initPopulation(sizeOfPopulation);
    int bestOne = INT_MAX;
    for (int i = 0; i < numberOfPopulations; i++) {
        createNewPopulation(numberOfChildren, probability, numberOfGenes);
        deleteRunt(sizeOfPopulation);
        int tempCost = matrix->costPermutation(population->at(0));
        if (bestOne > tempCost) {
            bestOne = tempCost;
        }
    }
    deleteRunt(0);
    delete population;
    return bestOne;
}

void GeneticAlgorithm::initPopulation(int sizeOfPopulation) {
    for (int i = 0; i < sizeOfPopulation; i++) {
        int* temp = generatePermutation(this->size);
        population->push_back(temp);
    }
    sortPopulation();
}

void GeneticAlgorithm::sortPopulation() {
    sort(population->begin(), population->end(), *this);
}

void GeneticAlgorithm::createNewPopulation(int number, int probability, int numberOfGenes) {
    vector <int*> * parents;
    vector <int*> * offspring = new vector<int*>();
    int created = 0;
    int* child;
    bool crossLider = true;
    do {
        parents = chooseParents();
        if (crossLider == true) {
            child = cross(population->at(0), population->at(1));
        } else {
            child = cross(parents->at(0), parents->at(1));
        }
        mutation(probability, numberOfGenes, child);
        if (!hasDuplicate(child, population) && !hasDuplicate(child, offspring)) {
            offspring->push_back(child);
            created++;
            crossLider = false;
        } else {
            delete[] parents->at(0);
            delete[] parents->at(1);
            delete parents;
            delete[] child;
        }
    } while (created < number);
    vector<int*>::iterator it;
    int i = 0;
    for (it = offspring->begin(); it != offspring->end(); ++it) {
        population->push_back(*it);
    }
    delete offspring;
    delete[] parents->at(0);
    delete[] parents->at(1);
    delete parents;
}

vector<int*>* GeneticAlgorithm::chooseParents() {
    vector<int*>* parents = new vector<int*>();
    calculateAllCosts(population);
    vector<int*>::iterator it;
    for (int i = 0; i < 2; i++) {
        float rand = std::rand() / (double)(RAND_MAX)* costOfAllPopulation;;
        float cost1 = 0;
        float cost2 = 0;
        for (it = population->begin(); it != population->end(); ++it) {
            cost2 += 1.0 / (float)matrix->costPermutation(*it);
            if (rand <= cost2) {
                int* temp = new int[size]();
                for (int j = 0; j < size; j++) {
                    temp[j] = (*it)[j];
                }
                if (!hasDuplicate(temp, parents)) {
                    parents->push_back(temp);
                    break;
                } else {
                    i--;
                    delete[] temp;
                    break;
                }
            } else {
                cost1 = cost2;
            }
        }
    }
    return parents;
}

void GeneticAlgorithm::calculateAllCosts(vector<int*>* population) {
    vector<int*>::iterator it;
    costOfAllPopulation = 0;
    for (it = population->begin(); it != population->end(); ++it) {
        this->costOfAllPopulation += 1.0 / (float)matrix->costPermutation(*it);
    }
}

void GeneticAlgorithm::mutation(int probability, int numberOfGenes, int* permutation) {
    int rand = std::rand() % probability;
    if (rand == 0) {
        for (int i = 0; i < numberOfGenes; i++) {
            int A = std::rand() % size;
            int B = std::rand() % size;
            swap(permutation[A], permutation[B]);
        }
    }
}

int * GeneticAlgorithm::generatePermutation(int size) {
    int * permutation = new int[size];
    for (int i = 0; i < size; i++) {
        permutation[i] = i;
    }
    random_shuffle(&permutation[0], &permutation[size]);
    return permutation;
}

void GeneticAlgorithm::deleteRunt(int size) {
    sortPopulation();
    vector<int*>::iterator it;
    while (population->size()>size) {
        it = population->end();
        it--;
        delete  (*it);
        population->erase(it);
    }
}

void GeneticAlgorithm::setParameters(int sizeOfPopulation, int numberOfPopulation, int numberOfGenes, int probability, int numberOfChild) {
    this->sizeOfPopulation = sizeOfPopulation;
    this->numberOfPopulations = numberOfPopulation;
    this->numberOfGenes = numberOfGenes;
    this->probability = probability;
    this->numberOfChildren = numberOfChild;
}

GeneticAlgorithm::~GeneticAlgorithm() {
    //delete matrix;
    //size = 0;
    ///delete[] bestTour;
    //if(!population->empty()) population->clear();
}

string GeneticAlgorithm::bestPathToString() {
    string result = "";
    for (int i = 0; i < size; i++) {
        if (i < size - 1)
            result += bestPath[i] + "  -> ";
        else
            result += bestPath[i];
    }
    return result;
};