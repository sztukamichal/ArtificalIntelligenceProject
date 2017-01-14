#pragma once
#include "Macierz.h"
#include "TabuList.h"
#include <time.h>
#include <string>

class Tabu_search
{

private:
	// DANE ORAZ SZUKANE PROBLEMU
	Macierz * matrix;				// macierz 
	TabuList *tabu;					// lista tabu
	int size;						// ilosc miast
	int *bestTour;					// najlepsza permutacja
	int minCost;					// minimalny koszt podrozy
	bool solved;					// parametr okreslajacy czy problem zostal rozwiazany

	//PARAMETRY ALGORYTMU
	int iterations;					// liczba iteracji petli glownej algorytmu
	int not_change;					// maksymalna liczba iteracji bez poprawy rozwiazania jako warunek koncowy
	int div_not_change;				// maksymalna liczba iteracji bez poprawy rozwiazania dla stosowania dywersyfikacji
	double alg_time;				// czas dzialania algorytmu
	int num_of_candidates;			// liczba kandydatów 
	int tabu_length;				// dlugosc listy tabu
	bool diverisfication_is_On;		// 0 - nie stosujemy dywersyfikacji, 1 - stosujemy dywersyfikacje 
	int stop_condition;				// 0 - iteracje , 1 - czas , 2 - not change

public:

	void setMinimalTour(int*);										// ustawia najlepsze rozwiazanie
	int algorithm();												// algorytm
	int* randomPermutation();										// losowa permutacja
	int* chooseNeighbour(int, const int*, int*, int*, int*);		// wybiera sasiadow sposrod num_of_candidates
	int* diversification();

	void showPermutation(const int*);								// wyswietl permutacje
	void showMatrix(){ matrix->show_matrix(); };						// wyswietl macierz


	//SETTERY				
	void setIterations(int i){ iterations = i; };
	void setNot_change(int n){ not_change = n; };
	void setDiv_not_change(int n){ div_not_change = n; };
	void setTime(double t){ alg_time = t; };
	void setNum_of_candidates(int k){ num_of_candidates = k; };
	void setTabu_length(int l){ tabu_length = l; };
	void setDiverification(bool on){ diverisfication_is_On = on; };
	void setStop_condition(int stop){ stop_condition = stop; };
	void setParameters(int iterations, int not_change, int div_not_change, double time, int num_of_candidates, int t_length, bool div, int stop);

	// GETTERY
	Macierz* getMatrix(){ return matrix; };
	int getIterations(){ return iterations; };
	int getNot_change(){ return not_change; };
	double getTime(){ return alg_time; };
	int getNum_of_candidates(){ return num_of_candidates; };
	int getTabu_lenght(){ return tabu->getLength(); };
	int getDiv_status(){ return diverisfication_is_On; };
	int getStop_condition(){ return stop_condition; };

	bool is_Solved(){ return solved; };
	int getMinCost(){ return minCost; };
	int getSize(){ return size; };
	int *getSolution(){ return bestTour; };

	Tabu_search(void);
	Tabu_search(std::string filename);
	~Tabu_search(void);
};

