#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <time.h>
#include <cstdlib>
#include "Matrix.h"

using namespace std;

int Matrix::loadDimension(string filename) {
	ifstream plik;
	plik.open(filename);
	string temp;
	int dimension;
	while (true) {
		if (plik.good()) {
			plik >> temp;
			if (temp == "DIMENSION:") {
				plik >> dimension;
				return dimension;
			}
		}
		else break;
	}
	return 0;
}

void Matrix::loadMatrix(string filename) {
	ifstream plik;
	plik.open(filename);
	string temp;
	while (true) {
		if (plik.good()) {
			plik >> temp;
			if (temp == "EDGE_WEIGHT_SECTION")
				break;
		}
		else break;
	}
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			plik >> matrix[i][j];
		}
	}
}

void Matrix::show_matrix() {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			cout.width(3);
			if (i == j)cout << "N";
			else
				cout << matrix[i][j];
		}
		cout << endl;
	}
}

int Matrix::costPermutation(int * permutation) {
	int cost = 0;
	for (int i = 0; i < size - 1; i++) {
		cost += matrix[permutation[i]][permutation[i + 1]];
	}
	cost += matrix[permutation[size - 1]][permutation[0]];
	return cost;
}

Matrix::Matrix(string filename) {
	this->size = loadDimension(filename);
	matrix = new int*[size];
	for (int i = 0; i < size; i++) {
		matrix[i] = new int[size];
	}
	loadMatrix(filename);
}

Matrix::Matrix(int size) {
	this->size = size;
	matrix = new int*[size];
	for (int i = 0; i < size; i++) {
		matrix[i] = new int[size];
	}
}

Matrix::~Matrix() {
	for (int i = 0; i < size; i++) {
		delete[] matrix[i];
	}
	delete[] matrix;
	size = 0;
}
