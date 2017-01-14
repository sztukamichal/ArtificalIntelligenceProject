#pragma once
#include "Macierz.h"
#include <string>
class SimulatedAnnealing
{
private:

	Macierz * macierz;
	int size;
public:

	int algorithm(int period, double alpa, double& temperature, double endTemperature);
	double initialTemperature();
	bool probability(int delta, double temperature);
	int* randomPermutation();
	void swapPermutation(const int * permutation, int* nextState);
	int* setMinimalTour(const int* permutation);
	int cost(int* permutation);
	void showPermutation(const int*);
	Macierz* getMacierz(){ return macierz; }
	int getSize(){ return size; };
	SimulatedAnnealing(std::string filename, bool choose);
	SimulatedAnnealing();
	~SimulatedAnnealing();
};

