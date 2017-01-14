#pragma once
#include <string>
// klasa przechowujaca macierz s�siedztwa 

class Macierz
{

private:
	int** matrix;
	int size;

public:

	int loadDimension(std::string filename);	// wczytywanie rozmiaru macierzy z pliku atsp
	void loadMatrix(std::string filename);		// wczytywanie macierzy z pliku atsp
	bool loadFromFile(std::string filename);	// wczytywanie macierzy z pliku txt
	void generate(int begin, int end);			//funkcja generujaca losowe odleg�osci z przedzia�u
	void show_matrix();							//funkcja wyswietlajaca macierz
	int costPermutation(int* permutation);		// zwraca koszt podrozy wg permutacji
	int getSize(){ return size; };
	Macierz(int size);							//konstruktor z podanna ilo�cia miast
	Macierz(std::string filename, bool choose);	// choose - okresla z jakiego pliku wczytac macierz, true- plik atsp, false - plik txt
	Macierz();
	~Macierz();
};

