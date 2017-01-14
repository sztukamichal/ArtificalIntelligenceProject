#pragma once
#include <string>
// klasa przechowujaca macierz s�siedztwa 

class Matrix
{

private:
	int** matrix;
	int size;

public:

	int loadDimension(std::string filename);	// wczytywanie rozmiaru macierzy z pliku atsp
	void loadMatrix(std::string filename);		// wczytywanie macierzy z pliku atsp
	void generate(int begin, int end);			//funkcja generujaca losowe odleg�osci z przedzia�u
	void show_matrix();							//funkcja wyswietlajaca macierz
	int costPermutation(int* permutation);		// zwraca koszt podrozy wg permutacji
	int getSize(){ return size; };
	Matrix(int size);							//konstruktor z podanna ilo�cia miast
	Matrix(std::string filename);	// choose - okresla z jakiego pliku wczytac macierz, true- plik atsp, false - plik txt
	Matrix();
	~Matrix();
};

