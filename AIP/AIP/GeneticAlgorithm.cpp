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
    int startPos;
    int endPos;
    /*cout << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    for (int i = 0; i < size; i++) {
        cout << "\t" << permutationA[i];
    }
    cout << endl;
    cout << endl;
    for (int i = 0; i < size; i++) {
        cout << "\t" << permutationB[i];
    }*/
    startPos = rand() % size;
    endPos = rand() % size;

    //cout << endl;
    //cout << endl << "\t start: " << startPos << " end " << endPos;
    //cout << endl;

    for (int i = 0; i < size; i++) {
        if (startPos < endPos && i > startPos && i < endPos) {
            child[i] = permutationA[i];
        } else if (startPos > endPos) {
            if (!(i < startPos && i > endPos)) {
                child[i] = permutationA[i];
            }
        }
    }
    /* cout << endl;
     cout << endl;
     for (int i = 0; i < size; i++) {
         cout << "\t" << child[i];
     }
     cout << endl;*/
    if (blackAndWhite) {
        int searchFrom = 0;
        bool found = false;
        for (int i = 0; i < size; i++) {
            if (!isInPermutation(child, permutationB[i])) {
                if (permutationB[i] % 2 == 0) {
                    for (int ii = searchFrom; ii < size; ii += 2) {
                        if (child[ii] == -1) {
                            found = true;
                            child[ii] = permutationB[i];
                            searchFrom = ii + 1;
                            searchFrom > size - 1 ? 1 : searchFrom;
                            break;
                        }
                    }
                    if (found == false) {
                        for (int ii = 0; ii < size; ii += 2) {
                            if (child[ii] == -1) {
                                child[ii] = permutationB[i];
                                searchFrom = ii + 1;
                                searchFrom > size - 1 ? 1 : searchFrom;
                                break;
                            }
                        }
                    }
                } else {
                    for (int ii = searchFrom; ii < size; ii += 2) {
                        if (child[ii] == -1) {
                            found = true;
                            child[ii] = permutationB[i];
                            searchFrom = ii + 1;
                            searchFrom > size - 1 ? 0 : searchFrom;
                            break;
                        }
                    }
                    if (found == false) {
                        for (int ii = 1; ii < size; ii += 2) {
                            if (child[ii] == -1) {
                                child[ii] = permutationB[i];
                                searchFrom = ii + 1;
                                searchFrom > size - 1 ? 1 : searchFrom;
                                break;
                            }
                        }
                    }
                }
            } else if (permutationB[i] % 2 == 0)
                searchFrom = 1;
            else
                searchFrom = 0;
            found = false;
        }
    } else {
        for (int i = 0; i < size; i++) {
            if (!isInPermutation(child, permutationB[i])) {

                for (int ii = 0; ii < size; ii++) {
                    if (child[ii] == -1) {
                        child[ii] = permutationB[i];
                        break;
                    }
                }
            }
        }
    }

    /*cout << endl;
    for (int i = 0; i < size; i++) {
        cout << "\t" << child[i];
    }*/

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
        createNewPopulation();
        deleteRunt(sizeOfPopulation);
        int tempCost = matrix->costPermutation(population->at(0));
        if (bestOne > tempCost) {
            bestOne = tempCost;
            delete[]bestPath;
            bestPath = copyPermutation(population->at(0));
        }
    }
    deleteRunt(0);
    population->clear();
    delete population;
    return bestOne;
}

void GeneticAlgorithm::initPopulation(int sizeOfPopulation) {
    for (int i = 0; i < sizeOfPopulation; i++) {
        int* temp = getRandomPermutation();
        population->push_back(temp);
    }
    sortPopulation();
}

void GeneticAlgorithm::sortPopulation() {
    sort(population->begin(), population->end(), *this);
}

void GeneticAlgorithm::createNewPopulation() {
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
        mutation(child);
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
    } while (created < numberOfChildren);
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
        float rand = std::rand() / (float)(RAND_MAX)* costOfAllPopulation;;
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

void GeneticAlgorithm::mutation(int* child) {
    int rand = std::rand() % probability;
    int cityA;
    int cityB;
    if (rand == 0) {
        for (int i = 0; i < numberOfGenes; i++) {
            setTwoRandomCities(cityA, cityB);
            swap(child[cityA], child[cityB]);
        }
    }
}

int * GeneticAlgorithm::getRandomPermutation() {
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
                evenSet[i / 2] = i;
            } else
                oddSet[(i - 1) / 2] = i;
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

string GeneticAlgorithm::bestPathToString() {
    ostringstream resultStream;
    int j = 0;
    int i = 0;
    // because then it begins and ends with even value, so it must be display as it is, not from 0 city
    if (blackAndWhite && size % 2 == 1) {
        for (i=0; i < size; i++) {
            if (i < size - 1) {
                resultStream << bestPath[i];
                resultStream << " -> ";
            } else
                resultStream << bestPath[i];
        }
    } else {
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
    }
    return resultStream.str();
};

void GeneticAlgorithm::setTwoRandomCities(int & cityA, int & cityB) {
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

int* GeneticAlgorithm::copyPermutation(const int * permutation) {
    int * next = new int[size];
    for (int i = 0; i < size; i++) {
        next[i] = permutation[i];
    }
    return next;
}

GeneticAlgorithm::~GeneticAlgorithm() {
    //delete matrix;
    //size = 0;
    ///delete[] bestTour;
    //if(!population->empty()) population->clear();
}
