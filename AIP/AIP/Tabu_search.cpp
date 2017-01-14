#include "stdafx.h"
#include "Tabu_search.h"
#include <string>
#include <algorithm>

using namespace std;

Tabu_search::Tabu_search(string filename)
{
	matrix = new Macierz(filename);
	size = matrix->getSize();
	bestTour = new int[size];
	for (int i = 0; i<size; i++)
	{
		bestTour[i] = 0;
	}
	minCost = 0;
	solved = false;
}

Tabu_search::Tabu_search(void)
{
}

Tabu_search::~Tabu_search(void)
{
	delete matrix;
	size = 0;
	delete[] bestTour;
	minCost = 0;
	solved = false;

	iterations = 0;
	not_change = 0;
	alg_time = 0;
	num_of_candidates = 0;
	tabu_length = 0;
	diverisfication_is_On = false;
}

void Tabu_search::setParameters(int i, int n, int divn, double t, int k, int t_length, bool div, int stop)
{
	iterations = i;
	not_change = n;
	div_not_change = divn;
	alg_time = t;
	num_of_candidates = k;
	tabu_length = t_length;
	diverisfication_is_On = div;
	stop_condition = stop;
}

int Tabu_search::algorithm()
{
	int i = 0;
	int wasBetter = 0;
	int stop_wasBetter = 0;
	int *begin = new int(0);
	int *end = new int(0);
	int *tempCost = new int(0);

	this->tabu = new TabuList(tabu_length);

	double toStop = 0;
	time_t start_timer, end_timer;

	int* currentPermutation = randomPermutation();
	setMinimalTour(currentPermutation);
	minCost = matrix->costPermutation(bestTour);

	time(&start_timer);
	while ((i<iterations || stop_condition != 0) && (toStop<(double)alg_time || stop_condition != 1) && (stop_wasBetter<not_change || stop_condition != 2))//g³owna petla
	{
		if (diverisfication_is_On && wasBetter == div_not_change)
		{
			delete[] currentPermutation;
			currentPermutation = diversification();
			wasBetter = 0;
		}
		currentPermutation = chooseNeighbour(num_of_candidates, currentPermutation, begin, end, tempCost);
		if (*tempCost < minCost)
		{
			minCost = *tempCost;
			setMinimalTour(currentPermutation);
			wasBetter = 0;
			stop_wasBetter = 0;
			tabu->add(new Move(*begin, *end));
		}
		else
		{
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

int* Tabu_search::chooseNeighbour(int k, const int* currentPermutation, int* begin, int* end, int* cost)
{
	int minimalCost = INT_MAX;				// zmienna na minimalny koszt wsrod s¹siadów
	int tempCost = INT_MAX;
	int* position = new int[size]();
	int* bestNeighbour = new int[size]();	// najlepsza permutacja wsrod sasiadow
	int* current = new int[size]();			// bierzace rozwiazanie
	for (int i = 0; i < size; i++)
	{
		current[i] = currentPermutation[i];
		position[currentPermutation[i]] = i;
	}

	int a, b;								// losowo zamieniane miasta
	for (int i = 0; i < k; i++)
	{
		do{
			a = rand() % size;
			b = rand() % size;
		} while (a == b);

		std::swap(current[a], current[b]);
		std::swap(position[current[b]], position[current[a]]);
		tempCost = matrix->costPermutation(current);

		if (tabu->isAllowed(position, new Move(a, b)) || (tempCost<minCost))	// czy ruch jest dozwolony
		{
			if (i != 0 && tempCost < minimalCost)
			{

				minimalCost = tempCost;
				for (int i = 0; i < size; i++)
				{
					bestNeighbour[i] = current[i];
				}
			}
			else if (i == 0)
			{
				minimalCost = tempCost;
				for (int i = 0; i < size; i++)
				{
					bestNeighbour[i] = current[i];
				}
			}
			*begin = position[current[a]];
			*end = position[current[b]];
		}
		std::swap(current[a], current[b]);
		std::swap(position[current[b]], position[current[a]]);
	}
	delete[] currentPermutation;
	delete[] position;
	delete[] current;
	*cost = minimalCost;
	return bestNeighbour;
}

int *Tabu_search::randomPermutation()
{
	int * permutation = new int[size];
	for (int i = 0; i < size; i++)
	{
		permutation[i] = i;
	}
	random_shuffle(&permutation[0], &permutation[size]);

	return permutation;
}

int* Tabu_search::diversification()
{
	int minimalCost = INT_MAX;				// zmienna na minimalny koszt wsrod rozwiazan
	int* bestOne = new int[size]();			// najlepsza losowa permutacja znaleziona dotychczas
	int* current = new int[size]();			// bierzace rozwiazanie
	int currentCost = INT_MAX;

	bestOne = randomPermutation();
	minimalCost = matrix->costPermutation(bestOne);
	for (int i = 0; i < 10; i++)
	{
		current = randomPermutation();
		currentCost = matrix->costPermutation(current);
		if (currentCost<minimalCost)
		{
			delete[] bestOne;
			//bestOne = new int[size]();
			bestOne = current;
			minimalCost = currentCost;
		}
	}
	return bestOne;
}

void Tabu_search::setMinimalTour(int * permutation){
	delete[] bestTour;
	//bestTour = permutation;
	bestTour = new int[size];
	for (int i = 0; i < size; i++)
	{
		bestTour[i] = permutation[i];
	}
}

void Tabu_search::showPermutation(const int * permutation){
	cout << endl << endl;
	for (int i = 0; i < size; i++)
	{
		std::cout << permutation[i] << "  ";
	}

	std::cout << endl;
}
