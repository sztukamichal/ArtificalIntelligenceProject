#include "stdafx.h"
#include "TabuSearch.h"

using namespace std;

TabuSearch::TabuSearch(string filename) {
    matrix = new Matrix(filename);
    size = matrix->getSize();
    bestPath = new int[size];
    for (int i = 0; i<size; i++) {
        bestPath[i] = 0;
    }
    minCost = 0;
    solved = false;


    iterations = 50000;					// liczba iteracji petli glownej algorytmu
    not_change = this->getSize() * 4;					// maksymalna liczba iteracji bez poprawy rozwiazania
    div_not_change = this->getSize() * 2;
    alg_time = 1;				// czas dzialania algorytmu
    num_of_candidates = this->getSize();			// liczba kandydatów
    tabu_length = 10;				// dlugosc listy tabu
    diverisfication_is_On = true;			// 0 - wylaczona 1 - wlaczona
    stop_condition = 0;				// 0 - interacje, 1 - time, 2 - not_change

}

TabuSearch::~TabuSearch(void) {
    delete matrix;
    size = 0;
    delete[] bestPath;
    minCost = 0;
    solved = false;

    iterations = 0;
    not_change = 0;
    alg_time = 0;
    num_of_candidates = 0;
    tabu_length = 0;
    diverisfication_is_On = false;
}

void TabuSearch::setParameters(int i, int n, int divn, double t, int k, int t_length, bool div, int stop) {
    iterations = i;
    not_change = n;
    div_not_change = divn;
    alg_time = t;
    num_of_candidates = k;
    tabu_length = t_length;
    diverisfication_is_On = div;
    stop_condition = stop;
}

int TabuSearch::algorithm() {
    int i = 0;
    int wasBetter = 0;
    int stop_wasBetter = 0;
    int *begin = new int(0);
    int *end = new int(0);
    int *tempCost = new int(0);

    this->tabu = new TabuList(tabu_length);

    double toStop = 0;
    time_t start_timer, end_timer;

    int* currentPermutation = getRandomPermutation();
    setMinimalPath(currentPermutation);
    minCost = matrix->costPermutation(bestPath);

    time(&start_timer);
    while ((i<iterations || stop_condition != 0) && (toStop<(double)alg_time || stop_condition != 1) && (stop_wasBetter<not_change || stop_condition != 2)) {
        if (diverisfication_is_On && wasBetter == div_not_change) {
            delete[] currentPermutation;
            currentPermutation = diversification();
            wasBetter = 0;
        }
        currentPermutation = chooseNeighbour(num_of_candidates, currentPermutation, begin, end, tempCost);
        if (*tempCost < minCost) {
            minCost = *tempCost;
            setMinimalPath(currentPermutation);
            wasBetter = 0;
            stop_wasBetter = 0;
            tabu->add(new Move(*begin, *end));
        } else {
            stop_wasBetter++;
            wasBetter++;
        }
        i++;
        time(&end_timer);
        toStop = difftime(end_timer, start_timer);
    }
    delete begin;
    delete end;
    delete tempCost;
    delete tabu;
    delete[] currentPermutation;
    solved = true;
    return minCost;
}

int* TabuSearch::chooseNeighbour(int k, const int* currentPermutation, int* begin, int* end, int* cost) {
    int minimalCost = INT_MAX;				// zmienna na minimalny koszt wsrod s¹siadów
    int tempCost = INT_MAX;
    int* position = new int[size]();
    int* bestNeighbour = new int[size]();	// najlepsza permutacja wsrod sasiadow
    int* current = new int[size]();			// biezace rozwiazanie
    for (int i = 0; i < size; i++) {
        current[i] = currentPermutation[i];
        position[currentPermutation[i]] = i;
    }

    int a, b;								// losowo zamieniane miasta
    for (int i = 0; i < k; i++) {

        setTwoRandomCities(a, b);
        swap(current[a], current[b]);
        swap(position[current[b]], position[current[a]]);
        tempCost = matrix->costPermutation(current);

        if (tabu->isAllowed(position, new Move(a, b)) || (tempCost<minCost)) {	// czy ruch jest dozwolony
            if (i != 0 && tempCost < minimalCost) {
                minimalCost = tempCost;
                for (int i = 0; i < size; i++) {
                    bestNeighbour[i] = current[i];
                }
            } else if (i == 0) {
                minimalCost = tempCost;
                for (int i = 0; i < size; i++) {
                    bestNeighbour[i] = current[i];
                }
            }
            *begin = position[current[a]];
            *end = position[current[b]];
        }
        swap(current[a], current[b]);
        swap(position[current[b]], position[current[a]]);
    }
    delete[] currentPermutation;
    delete[] position;
    delete[] current;
    *cost = minimalCost;
    return bestNeighbour;
}

int*  TabuSearch::getRandomPermutation() {
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

string TabuSearch::bestPathToString() {
    ostringstream resultStream;
    int j = 0;
    int i = 0;
    // because then it begins and ends with even value, so it must be display as it is, not from 0 city
    if (blackAndWhite && size % 2 == 1) {
        for (i = 0; i < size; i++) {
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

void TabuSearch::setTwoRandomCities(int & cityA, int & cityB) {
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

int* TabuSearch::diversification() {
    int minimalCost = INT_MAX;				// zmienna na minimalny koszt wsrod rozwiazan
    int* bestOne = new int[size]();			// najlepsza losowa permutacja znaleziona dotychczas
    int* current = new int[size]();			// bierzace rozwiazanie
    int currentCost = INT_MAX;

    bestOne = getRandomPermutation();
    minimalCost = matrix->costPermutation(bestOne);
    for (int i = 0; i < 10; i++) {
        current = getRandomPermutation();
        currentCost = matrix->costPermutation(current);
        if (currentCost<minimalCost) {
            delete[] bestOne;
            bestOne = current;
            minimalCost = currentCost;
        }
    }
    return bestOne;
}

void TabuSearch::setMinimalPath(int * permutation) {
    delete[] bestPath;
    bestPath = new int[size];
    for (int i = 0; i < size; i++) {
        bestPath[i] = permutation[i];
    }
}

void TabuSearch::showPermutation(const int * permutation) {
    cout << endl << endl;
    for (int i = 0; i < size; i++) {
        cout << permutation[i] << "  ";
    }
    cout << endl;
}