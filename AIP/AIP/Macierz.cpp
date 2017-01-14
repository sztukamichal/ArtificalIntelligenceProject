#include "stdafx.h"

#include <iostream>
#include <fstream>

#include <time.h>
#include <cstdlib>
#include "Macierz.h"
using namespace std;

// wczytywanie plików .txt
bool Macierz::loadFromFile(string nazwa)
{
	bool ok = false;
	fstream plik;

	plik.open(nazwa, ios::in);
	if (plik.good())
	{
		plik >> this->size;
		// wczytujemy ilosc miast.
		matrix = new int*[size];
		for (int i = 0; i < size; i++)
			matrix[i] = new int[size];
		for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			plik >> matrix[i][j];
		ok = true;
		plik.close();
	}
	else
		cout << "Otwieranie pliku nie powiodlo sie." << endl;
	return ok;
}
// wczytywanie rozmiaru z pliku atsp
int Macierz::loadDimension(string filename){

	ifstream plik;

	plik.open(filename);
	string temp;
	int dimension;
	while (true)
	{
		if (plik.good()){
			plik >> temp;
			if (temp == "DIMENSION:")
			{
				plik >> dimension;
				return dimension;
			}
		}
		else break;
	}
	return 0;
}
// wczytywanie z pliku tsp
void Macierz::loadMatrix(string filename){


	ifstream plik;
	plik.open(filename);
	string temp;
	while (true)
	{
		if (plik.good()){
			plik >> temp;
			if (temp == "EDGE_WEIGHT_SECTION")
				break;
		}
		else break;
	}

	for (int i = 0; i < size; i++)
	for (int j = 0; j < size; j++)
	{
		plik >> matrix[i][j];
	}
}
// generowanie macierzy
void Macierz::generate(int begin, int end){
	int los = 0;
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (i == j) matrix[i][j] = INT_MAX;
			else{
				matrix[i][j] = begin + (rand() % (end - begin + 1));
			}
		}
	}

}
// wyswietlanie macierzy
void Macierz::show_matrix(){
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			cout.width(3);
			if (i == j)cout << "N";
			else
				cout << matrix[i][j];
		}
		cout << endl;
	}
}
// koszt podrozy
int Macierz::costPermutation(int * permutation){
	int cost = 0;
	for (int i = 0; i < size - 1; i++){
		cost += matrix[permutation[i]][permutation[i + 1]];
	}
	cost += matrix[permutation[size - 1]][permutation[0]];
	return cost;
}
// konstruktor z wczytywaniem z pliku
Macierz::Macierz(string filename, bool choose)
{
	if (choose)
	{
		this->size = loadDimension(filename);
		matrix = new int*[size];
		for (int i = 0; i < size; i++)
		{
			matrix[i] = new int[size];
		}

		loadMatrix(filename);
	}
	else loadFromFile(filename);
}

Macierz::Macierz(int size){
	this->size = size;
	matrix = new int*[size];
	for (int i = 0; i < size; i++)
	{
		matrix[i] = new int[size];
	}
}

Macierz::~Macierz()
{
	for (int i = 0; i<size; i++)
		delete[] matrix[i];

	delete[] matrix;
	size = 0;
}
