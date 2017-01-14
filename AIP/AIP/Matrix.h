#pragma once
#include <string>

class Matrix {

	private:
		int** matrix;
		int size;

	public:
		int loadDimension(std::string filename);	// wczytywanie rozmiaru macierzy z pliku atsp
		void loadMatrix(std::string filename);		// wczytywanie macierzy z pliku atsp
		void show_matrix();							// funkcja wyswietlajaca macierz
		int costPermutation(int* permutation);		// zwraca koszt podrozy wg permutacji
		int getSize(){ return size; };

		Matrix(int size);							// konstruktor z podanna iloœcia miast
		Matrix(std::string filename);	
		Matrix();
		
		~Matrix();
};

