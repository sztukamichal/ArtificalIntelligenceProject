#pragma once
#include "Matrix.h"
#include <string>
class SimulatedAnnealing {
	private:
		Matrix * matrix;
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
		Matrix* getMacierz(){ return matrix; }
		int getSize(){ return size; };

		SimulatedAnnealing(std::string filename);
		SimulatedAnnealing();
		
		~SimulatedAnnealing();
};

