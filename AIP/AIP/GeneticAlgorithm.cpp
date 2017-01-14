#include "stdafx.h"
#include "GeneticAlgorithm.h"

GeneticAlgorithm::GeneticAlgorithm(std::string filename) {
	matrix = new Matrix(filename);
	size = matrix->getSize();
	bestTour = new int[size];
	for (int i = 0; i<size; i++) {
		bestTour[i] = 0;
	}
	minCost = 0;
	solved = false;
	populationCost = 0;
}

void GeneticAlgorithm::showPopulation(vector<int*>* population) {
	vector<int*>::iterator it; int i = 0;
	cout << "\n";
	for (it = population->begin(); it != population->end(); ++it) {// if (i<10){
		cout << "OSOBNIK: " << i;
		showPermutation(*it);
		std::cout << "COST " << matrix->costPermutation(*it) << endl;//}
		i++;
	}
}

int* GeneticAlgorithm::cross(int* permutationA, int* permutationB){
	int* child = new int[size]();
	for (int i = 0; i < size; i++) {
		child[i] = -1;
	}
	// Get start and end sub tour positions for parent1's tour
	int startPos = rand() % size;
	int endPos = rand() % size;

	// Loop and add the sub tour from parent1 to our child
	for (int i = 0; i < size; i++) {
		// If our start position is less than the end position
		if (startPos < endPos && i > startPos && i < endPos) {
			child[i] = permutationA[i];
		} // If our start position is larger
		else if (startPos > endPos) {
			if (!(i < startPos && i > endPos)) {
				child[i] = permutationA[i];
			}
		}
	}
	// Loop through parent2's city tour
	for (int i = 0; i < size; i++) {
		// If child doesn't have the city add it
		if (!isInPermutation(child, permutationB[i])) {
			// Loop to find a spare position in the child's tour
			for (int ii = 0; ii < size; ii++) {
				// Spare position found, add city
				if (child[ii] == -1) {
					child[ii] = permutationB[i];
					break;
				}
			}
		}
	}
	return child;
}

void GeneticAlgorithm::showPermutation(const int* permutation) {
	cout << endl;
	for (int i = 0; i < size; i++) {
		std::cout << permutation[i] << "  ";
	}
}

bool GeneticAlgorithm::isInPermutation(const int* permutation, int city) {
	for (int i = 0; i < this->size; i++) {
		if (permutation[i] == city)
			return true;
	}
	return false;
}

vector<int*>* GeneticAlgorithm::chooseParents(){
	vector<int*>* parents = new vector<int*>();
	calculateAllCosts(population);
	//cout << "POPCOST" << populationCost << endl;
	vector<int*>::iterator it;
	for (int i = 0; i < 2; i++) {
		float rand = std::rand() / (double)(RAND_MAX)* populationCost;// +1;
		float cost1 = 0;
		float cost2 = 0;
		//int end = population->size()-1;
		for (it = population->begin(); it != population->end(); ++it) {
			cost2 += 1.0 / (float)matrix->costPermutation(*it);
			//      end--;
			//      cout << cost2 << endl;
			if (rand <= cost2) {
				int* temp = new int[size]();
				// to do
				// wylosowanie osobnika
				for (int j = 0; j < size; j++) {
					temp[j] = (*it)[j];
				}
				//      parents->push_back(temp);
				if (!hasDuplicate(temp, parents)){
					parents->push_back(temp);
					//delete[] temp;
					break;
				}
				else {
					i--;
					delete[] temp;
					break;
				}
			}
			else {
				cost1 = cost2;
			}
		}
	}
	return parents;
}

void GeneticAlgorithm::createNewPopulation(int number, int probability, int numberOfGenes){
	vector <int * > * parents;
	vector <int * > * offspring = new vector<int*>();
	int created = 0;
	int* child;
	bool crossLider = true;
	do {
		parents = chooseParents(); // wybranie dwóch róznych rodziców
		if (crossLider == true) {
			child = cross(population->at(0), population->at(1));
		}
		else {
			child = cross(parents->at(0), parents->at(1));
		}
		mutation(probability, numberOfGenes, child);
		if (!hasDuplicate(child, population) && !hasDuplicate(child, offspring)) {
			offspring->push_back(child);
			created++;
			crossLider = false;
		}
		else {
			delete[] parents->at(0);
			delete[] parents->at(1);
			delete parents;
			delete[] child;
		}
	} while (created < number);
	vector<int*>::iterator it; int i = 0;
	//cout << endl;
	//population->clear();
	for (it = offspring->begin(); it != offspring->end(); ++it) {
		population->push_back(*it);
		//      delete[] *it;
	}
	delete offspring;
	delete[] parents->at(0);
	delete[] parents->at(1);
	delete parents;
}

bool GeneticAlgorithm::hasDuplicate(int* permutation, vector<int *>* population){
	vector<int*>::iterator it;
	if (population->size() == 0) {
		return false;
	}
	for (it = population->begin(); it != population->end(); ++it) {
		int count = 0;
		for (int i = 0; i < size; i++) {
			if ((*it)[i] != permutation[i]){
				break;
			}
			count++;
		}
		if (count == size) {
			return true;
		}
	}
	return false;
}

void GeneticAlgorithm::crossover(int numberOfChild, vector<int*>* parents) {
	int rand1, rand2;
	int * offspring;
	for (int i = 0; i < numberOfChild; i++) {
		//d
		//{
		//      rand1 = rand() % parents->size();
		//      rand2 = rand() % parents->size();

		//} while (rand1==rand2);
		//      cout << "\nRODZIC1 " << endl;
		//      showPermutation(parents->at(0));
		//cout << "\nRODZIC2 " << endl;
		//   showPermutation(parents->at(1));
		//      cout << "\nDZIECKO " << endl;


		//      cout << "\nTO CROSS\n";
		//      showPermutation(parents->at(0));
		//      cout << "\n\n";
		//      showPermutation(parents->at(1));
		//      cout << "---------------\n";
		offspring = cross(parents->at(1), parents->at(0));
		//if (!hasDuplicate(offspring,this->population)){
		population->push_back(offspring);
		//}
		//else
		//      i--;
	}
}

void GeneticAlgorithm::calculateAllCosts(vector<int*>* population) {
	vector<int*>::iterator it;
	populationCost = 0;
	for (it = population->begin(); it != population->end(); ++it) {
		this->populationCost += 1.0 / (float)matrix->costPermutation(*it);
	}
}

int GeneticAlgorithm::algorithm(int sizeOfPopulation, int numberOfPopulation, int numberOfGenes, int probability, int numberOfChild) {       // matrix->show_matrix();
	population = new vector<int*>();
	initialPopulation(sizeOfPopulation);
	//showPopulation(this->population);
	int bestOne = INT_MAX;
	for (int i = 0; i < numberOfPopulation; i++){
		//      for (int j = 0; j < numbeOfChild; j++)
		//{    
		//      vector<int*> * parents = chooseParents(2);
		//      crossover(1,parents);
		//}
		createNewPopulation(numberOfChild, probability, numberOfGenes);
		deleteRunt(sizeOfPopulation);
		//      deleteRunt();
		int tempCost = matrix->costPermutation(population->at(0));
		if (bestOne > tempCost) {
			bestOne = tempCost;
			//showPopulation(this->population);
		}
		//      cout << bestOne << endl;
		//showPopulation(this->population);
	}
	//showPopulation(this->population);
	//      populationSize = 5;
	deleteRunt(0);
	//population->clear();
	delete population;
	return bestOne;
}

void GeneticAlgorithm::mutation(int probability, int numberOfGenes, int* permutation)
{
	int rand = std::rand() % probability;
	if (rand == 0) {
		for (int i = 0; i < numberOfGenes; i++){
			int A = std::rand() % size;
			int B = std::rand() % size;
			swap(permutation[A], permutation[B]);
		}
	}
}

void GeneticAlgorithm::initialPopulation(int sizeOfPopulation) {
	populationSize = sizeOfPopulation;
	for (int i = 0; i < populationSize; i++) {
		int* temp = generatePermutation(this->size);
		population->push_back(temp);
	}
	sortPopulation();
}

void GeneticAlgorithm::sortPopulation() {
	sort(population->begin(), population->end(), *this);
}

int * GeneticAlgorithm::generatePermutation(int size){
	int * permutation = new int[size];
	for (int i = 0; i < size; i++) {
		permutation[i] = i;
	}
	std::random_shuffle(&permutation[0], &permutation[size]);
	return permutation;
}

void GeneticAlgorithm::deleteRunt(int size) {
	sortPopulation();
	vector<int*>::iterator it;
	//showPopulation(this->population);
	while (population->size()>size) {
		it = population->end();
		it--;
		//--it;
		//cout << "----------------------";
		//showPermutation(*it);
		delete  (*it);//population->at(population->size()-1);
		//delete *it;
		//cout << "----------------------";
		//showPermutation(*it);
		population->erase(it);
	}
}

GeneticAlgorithm::~GeneticAlgorithm() {
	//delete matrix;	
	//size = 0;						
	///delete[] bestTour;	
	//if(!population->empty()) population->clear();
}