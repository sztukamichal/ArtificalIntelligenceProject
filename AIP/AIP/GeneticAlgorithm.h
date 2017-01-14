#pragma once
#include "Matrix.h"
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;
class GeneticAlgorithm
{

private:

	Matrix* matrix;
	vector<int*>* population;
	int populationSize;
	float populationCost;            // suma wartosci funkcji celu dla ca³ej populacji
	int size;                                               // ilosc miast
	int *bestTour;                                  // najlepsza permutacja
	int minCost;                                    // minimalny koszt podrozy
	bool solved;                                    // parametr okreslajacy czy problem zostal rozwiazany

	//PARAMETRYY ALGORYTMU

	int numberOfPopulation;  // liczba populacji algorytmu


public:

	bool operator() (int* & a, int* & b) const{

		return (matrix->costPermutation(a) < matrix->costPermutation(b));


	}

	//static bool cmparator(const int* i,const int *j){
	//return (matrix->costPermutation(this) > matrix->costPermutation(j));

	//};
	bool hasDuplicate(int*, vector<int *>* population);
	int algorithm(int sizeOfPopulation, int numberOfPopulation, int numberOfGenes, int probability, int numberOfChild); // funkcja algorytmu
	void initialPopulation(int);
	void sortPopulation();
	void deleteRunt(int);
	void showPopulation(vector<int*>* population);
	void showPermutation(const int*);
	bool isInPermutation(const int* permutation, int city);
	int* generatePermutation(int size);
	vector<int*>*chooseParents();
	void crossover(int numberOfChild, vector<int*>*);
	void createNewPopulation(int, int, int);
	void mutation(int probability, int numberOfGenes, int * permutation);
	int getSize(){ return size; };
	void showMatrix(){ matrix->show_matrix(); };

	void calculateAllCosts(vector<int*>* population);
	int* cross(int*, int*);
	// SETTERY


	//GETTERY

	Matrix* getMatrix(){ return matrix; };

	GeneticAlgorithm(std::string);
	~GeneticAlgorithm();
};