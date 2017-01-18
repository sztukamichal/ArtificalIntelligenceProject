#include "stdafx.h"
#include "TabuSearch.h"
#include "SimulatedAnnealing.h"
#include "GeneticAlgorithm.h"
#include <iostream>
#include <time.h>
#include <ctime>
#include <string>
#include <fstream>
#include <Windows.h>
#include <sstream> 

using namespace std;

LARGE_INTEGER startTimer() {
	LARGE_INTEGER start;
	DWORD_PTR oldmask = SetThreadAffinityMask(GetCurrentThread(), 0);
	QueryPerformanceCounter(&start);
	SetThreadAffinityMask(GetCurrentThread(), oldmask);
	return start;
}

LARGE_INTEGER endTimer() {
	LARGE_INTEGER stop;
	DWORD_PTR oldmask = SetThreadAffinityMask(GetCurrentThread(), 0);
	QueryPerformanceCounter(&stop);
	SetThreadAffinityMask(GetCurrentThread(), oldmask);
	return stop;
}

struct myfillandw
{
	myfillandw(char f, int w)
	: fill(f), width(w) {}

	char fill;
	int width;
};

std::ostream& operator<<(std::ostream& o, const myfillandw& a)
{
	o.fill(a.fill);
	o.width(a.width);
	return o;
}

//Funkcja obliczajaca czas w [s]
double duration(double &time, int repeat){
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	time = time / freq.QuadPart;
	time = time / repeat;
	return time;
}

void testGenetic();
void testAnnealing(string filename, int period, double  alphas[], double ile, double endTemperature);
void testTabuAtsp();
void geneticMenu(string);
void simulatedMenu(string);
void tabuMenu(string);

void printConsole(){
	cout << endl << "console> ";
}

string arrayToString(double array[], int size) {
	ostringstream stream;
	stream << "{";
	for (int i = 0; i<size; i++) {
		stream <<  array[i];
		if (i != size - 1)
			stream << ",";
	}
	stream << "}";
	return stream.str();
}

string arrayToString(int array[], int size) {
	ostringstream stream;
	stream << "{";
	for (int i = 0; i<size; i++) {
		stream << array[i];
		if (i != size - 1)
			stream << ",";
	}
	stream << "}";
	return stream.str();
}

string getCurrentTime() {
	time_t t = time(0);
	struct tm now;
	localtime_s(&now, &t);
	char buf[80];
	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &now);
	return buf;
}

string arrayToString(string array[], int size) {
	string result;
	result = "{";
	int posOfAtsp;
	for (int i = 0; i<size; i++) {
		posOfAtsp = array[i].find(".atsp");
		if (posOfAtsp != string::npos) {
			result += array[i].substr(0, posOfAtsp);
		}
		else{
			result += array[i];
		}
		if (i != size - 1)
			result += ",";
	}
	result += "}";
	return result;
}

inline bool fileExists(const std::string& name) {
	ifstream f(name.c_str());
	return f.good();
}

string mainMenu = 
"|.........................................................................|\n"
"|.......... ________________________________ .............................|\n"
"|..._______|________TSP PROBLEM SOLVER______|____________________.........|\n"
"|..|                                                             |........|\n"
"|..|   .....ALGORITHMS......                                     |........|\n"
"|..|                                                             |........|\n"
"|..|   1.SIMULATED ANNEALING                         S           |........|\n"
"|..|   2.TABU SEARCH                                 T           |........|\n"
"|..|   3.GENETIC ALGORITHM                           G           |........|\n"
"|..|                                                             |........|\n"
"|..|   ----- OPTION ------                                       |........|\n"
"|..|   4.LOAD FROM FILE (.atsp)                      L           |........|\n"
"|..|   5.PREVIEW LOADED DATA                         P           |........|\n"
"|..|                                                             |........|\n"
"|..|   7.QUIT                                        Q           |........|\n"
"|..|________________________AUTORZY______________________________|........|\n"
"|..|                                                             |........|\n"
"|..|   MICHAL SZTUKA                           200798            |........|\n"
"|..|   JOANNA TUROWSKA                         236088            |........|\n"
"|..|_____________________________________________________________|........|\n"
"|.........................................................................|\n"
"|.........................................................................|\n";

//funkcja zamieniaj�ca liczb� na string
string ltos(int);

TabuSearch* tabuSearch;
SimulatedAnnealing* simulatedAnnealing;
GeneticAlgorithm* geneticAlgorithm;

int _tmain(int argc, _TCHAR* argv[])
{
	void startProgram();
	srand((unsigned int)time(NULL));

	/*LARGE_INTEGER performanceCountStart, performanceCountEnd;
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	*/
	
	bool showMenu = true;
	bool isProperlyLoaded = false;
	char choice;
	string fileName;
	string defaultFileName = "br17.atsp";

	if (fileExists(defaultFileName)) {
		simulatedAnnealing = new SimulatedAnnealing(defaultFileName);
		tabuSearch = new TabuSearch(defaultFileName);
		geneticAlgorithm = new GeneticAlgorithm(defaultFileName);
		isProperlyLoaded = geneticAlgorithm->getSize() > 0;
	}

	while (1) {
		if (showMenu) {
			system("cls");
			cout << mainMenu << endl;
		}

		if (isProperlyLoaded) {
			cout << endl << "Loaded from default file: " << defaultFileName;
		} 
		else {
			cout << endl << "There was a problem with loading data. " << defaultFileName << " may not exist.";
		}

		showMenu = false;

		printConsole();
		cin >> choice;

		if (!isProperlyLoaded && choice != 'L' && choice != 'l' && choice != 'q' && choice != 'Q') {
			cout << endl << "Try to load data from another file.";
			//just to ommit switch clause
			choice = 'o';
		}
		else if (choice != 'l' && choice != 'L') {
			showMenu = true;
		}
		
		switch (choice) {
			case 's':
			case 'S':
				simulatedMenu(defaultFileName);
				break;
			case 't':
			case 'T':
				tabuMenu(defaultFileName);
				break;
			case 'g':
			case 'G':
				geneticMenu(defaultFileName);
				break;
			case 'l':
			case 'L':
				cout << endl << "Input filename : ";
				cin >> defaultFileName;
				if (defaultFileName.find(".atsp") != string::npos) {
				}
				else {
					defaultFileName += ".atsp";
				}
				
				if (fileExists(defaultFileName)) {
					delete simulatedAnnealing;
					delete tabuSearch;
					delete geneticAlgorithm;
					
					simulatedAnnealing = new SimulatedAnnealing(defaultFileName);
					tabuSearch = new TabuSearch(defaultFileName);
					geneticAlgorithm = new GeneticAlgorithm(defaultFileName);

					isProperlyLoaded = geneticAlgorithm->getSize() > 0;
				}
				else {
					cout << endl << "Some errors with file occured, check if this file exist.";
					isProperlyLoaded = false;
				}
				break;
			case 'p':
			case 'P':
				system("cls");
				cout << endl;
				tabuSearch->showMatrix();
				cout << endl << "Press any button to back...";
				cin.ignore();
				cin.get();
				system("cls");
				break;
			case 'Q':
			case 'q':
				exit(0);
			default:
				break;
		}
	}

	system("PAUSE");
	return 0;
}

void geneticMenu(string filename)
{
	ostringstream geneticMenuStream;
	string geneticMenu;
	char choice;
	bool goBack = false;
	bool showMenu = true;
	double time = 0;
	LARGE_INTEGER performanceCountStart, performanceCountEnd;
	int solution = 0;
	int newValue;


	while (!goBack){
		geneticMenuStream.str("");
		geneticMenuStream.clear();
		geneticMenuStream <<
			"|.........................................................................|\n"
			"|.......... ________________________________ .............................|\n"
			"|..........|      _  _   _                  |.............................|\n"
			"|..........|     / |/ | |_ |\\ | | |        |.............................|\n"
			"|..........|    /     | |_ | \\| |_|        |.............................|\n"
			"|..._______|________________________________|____________________.........|\n"
			"|..|                                                             |........|\n"
			"|..|                   GENETIC ALGORITHM                         |........|\n"
			"|..|                                                             |........|\n"
			"|..|      OPTION:                                   KEY:         |........|\n"
			"|..|                                                             |........|\n"
			"|..|      START SINGLE SOLUTION                      S           |........|\n"
			"|..|      MAKE TESTS                                 T           |........|\n"
			"|..|      CHANGE VALUE OF PARAMETER NR. X            X           |........|\n"
			"|..|      BACK                                       B           |........|\n"
			"|..|                                                             |........|\n"
			"|..|________________   INSTANCE   _______________________________|........|\n"
			"|..|                                                             |........|\n"
			"|..|      FILENAME :                        " << myfillandw(' ', 10) << filename << "           |........|\n"
			"|..|      NUMBER OF CITIES :                       " << myfillandw(' ', 3) << geneticAlgorithm->getSize() << "           |........|\n"
			"|..|                                                             |........|\n"
			"|..|_____________   PARAMETERS OF ALGORITHM   ___________________|........|\n"
			"|..|                                                             |........|\n"
			"|..|   1. SIZE OF POPULATION                       " << myfillandw(' ', 3) << geneticAlgorithm->sizeOfPopulation << "           |........|\n"
			"|..|   2. POPULATION QUANTITY                      " << myfillandw(' ', 3) << geneticAlgorithm->numberOfPopulation << "           |........|\n"
			"|..|   3. NUMBER OF GENES                          " << myfillandw(' ', 3) << geneticAlgorithm->numberOfGenes << "           |........|\n"
			"|..|   4. PROBABILITY OF MUTATION                  " << myfillandw(' ', 3) << geneticAlgorithm->probability << "           |........|\n"
			"|..|   5. CHILDREN QUANTITY                        " << myfillandw(' ', 3) << geneticAlgorithm->numberOfChild << "           |........|\n"
			"|..|_____________________________________________________________|........|\n"
			"|.........................................................................|\n"
			"|.........................................................................|\n"
			"|.........................................................................|\n"
			"|.........................................................................|\n";
		geneticMenu = geneticMenuStream.str();
		time = 0;
		if (showMenu) {
			system("cls");
			cout << geneticMenu;
		}
		printConsole();
		cin >> choice;
		switch (choice){
			case '1':
				cout << "Input new value for size of population (>0): ";
				cin >> newValue;
				if (newValue>0) geneticAlgorithm->sizeOfPopulation = newValue;
				break;
			case '2':
				cout << "Input new value for quantity of populations (>0): ";
				cin >> newValue;
				if (newValue>0) geneticAlgorithm->numberOfPopulation = newValue;
				break;
			case '3':
				cout << "Input new value for probability of mutation (0 < x <100): ";
				cin >> newValue;
				if (newValue>0) geneticAlgorithm->probability = newValue;
				break;
			case '4':
				cout << "Input new value for number of genes (>0): ";
				cin >> newValue;
				if (newValue>0) geneticAlgorithm->numberOfGenes = newValue;
				break;
			case '5':
				cout << "Input new value for number of children (>0): ";
				cin >> newValue;
				if (newValue>0) geneticAlgorithm->numberOfChild = newValue;
				break;
			case 's':
			case 'S':
				cout << "Please wait... Computing...";
				performanceCountStart = startTimer();
				solution = geneticAlgorithm->algorithm();
				performanceCountEnd = endTimer();
				time = (double)(performanceCountEnd.QuadPart - performanceCountStart.QuadPart);
				duration(time, 1);
				cout << "MIN: " << solution;
				cout << "\nDuration of computation: " << time << " [ms]" << endl;
				cout << endl << "Press any key to continue...";
				cin.ignore();
				cin.get();
				break;
			case 't':
			case 'T':
				testGenetic();
				break;
			case 'b':
			case 'B':
				goBack = true;
				break;
			default:
				break;
		}
		showMenu = choice != 'S' && choice != 's';
	}

};

void tabuMenu(string filename)
{
	LARGE_INTEGER performanceCountStart, performanceCountEnd;

	string menu = "|.........................................................................|\n"
		"|.......... ________________________________ .............................|\n"
		"|..........|      _  _   _                  |.............................|\n"
		"|..........|     / |/ | |_ |\\ | | |        |.............................|\n"
		"|..........|    /     | |_ | \\| |_|        |.............................|\n"
		"|..._______|________________________________|____________________.........|\n"
		"|..|                                                             |........|\n"
		"|..|                   TABU SEARCH                               |........|\n"
		"|..|                                                             |........|\n"
		"|..|   OPTION:                                      KEY:         |........|\n"
		"|..|                                                             |........|\n"
		"|..|   START SINGLE SOLUTION                         S           |........|\n"
		"|..|   MAKE TESTS                                    T           |........|\n"
		"|..|   GO BACK                                       X           |........|\n"
		"|..|_____________________________________________________________|........|\n"
		"|.........................................................................|\n"
		"|.........................................................................|\n"
		"|.........................................................................|\n"
		"|.........................................................................|\n";

	int option;
	bool loop = true;
	bool loop2 = true;
	bool loop3 = true;

	int size = tabuSearch->getSize();
	int iterations = 50000;					// liczba iteracji petli glownej algorytmu
	int not_change = size * 4;					// maksymalna liczba iteracji bez poprawy rozwiazania
	int div_not_change = size * 2;
	double alg_time = 1;				// czas dzialania algorytmu
	int num_of_candidates = size;			// liczba kandydat�w 
	int tabu_length = 10;				// dlugosc listy tabu
	bool diversificationOn = true;			// 0 - wylaczona 1 - wlaczona 
	int stop_condition = 0;				// 0 - interacje, 1 - time, 2 - not_change

	int solution = 0;
	while (loop3){
		system("cls");
		cout << menu;
		cout << "Wczytany plik: " << filename << endl;
		cin >> option;
		double time = 0;
		switch (option){
		case 1://menu1
			loop2 = true;
			while (loop2){
				system("cls");
				cout << "\n --- Aktualne parametry algorytmu ---\n\n";
				cout << "SIZE : " << size << endl;
				cout << "Warunek stopu: ";
				if (stop_condition == 0) cout << " Liczba iteracji\n";
				else if (stop_condition == 1) cout << " Czas\n";
				else if (stop_condition == 2) cout << " Brak zmiany rozwiazania\n";
				if (stop_condition == 0) cout << "Liczba iteracji : " << iterations << endl;
				else if (stop_condition == 1) cout << "Czas : " << alg_time << endl;
				else if (stop_condition == 2) cout << "Brak zmiany od : " << not_change << endl;
				cout << "Stosowanie dywersyfikacji: ";
				if (diversificationOn) cout << " tak" << endl;
				else cout << " nie" << endl;
				if (diversificationOn) cout << "Parametr not_change dla dywersyfikacji: " << div_not_change << endl;
				cout << "Dlugosc listy tabu : " << tabu_length << "\n";
				cout << "Liczba kandydatow : " << num_of_candidates << endl;
				cout << "1. Wyswietl macierz sasiedztwa\n2. Edytuj parametry\n3. Rozpocznij alogrytm\n4. Powrot\n";
				cin >> option;
				switch (option)
				{
				case 1:
					cout << endl;
					tabuSearch->showMatrix();
					cout << endl << "Nacisnij dowolny klawisz aby kontynuowac...";
					cin.ignore();
					cin.get();
					break;
				case 2:
					loop = true;
					while (loop)
					{
						system("cls");
						cout << "\n --- Aktualne parametry algorytmu ---\n\n";
						cout << "SIZE : " << size << endl;
						cout << "Warunek stopu(1): ";
						if (stop_condition == 0) cout << " Liczba iteracji\n";
						else if (stop_condition == 1) cout << " Czas\n";
						else if (stop_condition == 2) cout << " Brak zmiany rozwiazania\n";
						if (stop_condition == 0) cout << "Liczba iteracji(2) : " << iterations << endl;
						else if (stop_condition == 1) cout << "Czas(2) : " << alg_time << endl;
						else if (stop_condition == 2) cout << "Brak zmiany od(2) : " << not_change << endl;
						cout << "Stosowanie dywersyfikacji(3) : ";
						if (diversificationOn) cout << " tak" << endl;
						else cout << " nie" << endl;
						if (diversificationOn) cout << "Parametr not_change dla dywersyfikacji(4): " << div_not_change << endl;
						cout << "Dlugosc listy tabu(5) : " << tabu_length << "\n";
						cout << "Liczba kandydatow(6) : " << num_of_candidates << endl;
						cout << "\nPowrot(7)\n\nOpcja nr: ";
						int ktory;
						cin >> ktory;
						cout << "\n";
						switch (ktory)
						{
						case 1:
							cout << "Podaj odpowiedni warunek stopu : \n 0 - interacje, 1 - czas, 2 - brak zmiany rozwiazania\n";
							cin >> ktory;
							if (ktory >= 0 && ktory <3) stop_condition = ktory;
							break;
						case 2:
							if (stop_condition == 0)
							{
								cout << "Podaj liczbe iteracji\n";
								cin >> ktory;
								if (ktory>10) iterations = ktory;
							}
							else if (stop_condition == 1)
							{
								cout << "Podaj czas \n";
								double czas;
								cin >> czas;
								if (czas>0.0) alg_time = czas;
							}
							else if (stop_condition == 2)
							{
								cout << "Brak zmiany od ilu iteracji \n";
								cin >> ktory;
								if (ktory > 0)not_change = ktory;
							}
							break;
						case 3:
							cout << "Czy stosowac dywersyfikacje : \n 0 - nie , 1 - tak\n";
							cin >> ktory;
							if (ktory == 0) diversificationOn = false;
							else diversificationOn = true;
							break;
						case 4:
							do{
								cout << "Podaj max liczbe iteracji bez poprawy rozwiazania dla dywersyfikacji (>0) : ";
								cin >> ktory;
							} while (ktory <= 0);
							div_not_change = ktory;
							break;
						case 5:
							do{
								cout << "Podaj dlugosc listy tabu : ";
								cin >> ktory;
							} while (ktory <= 0);
							tabu_length = ktory;
							break;
						case 6:
							cout << "Ilu kandydatow\n";
							cin >> ktory;
							if (ktory>1) num_of_candidates = ktory;
							break;
						/*case 7:
							tabu_search->setParameters(iterations, not_change, div_not_change, alg_time, num_of_candidates, tabu_length, diversificationOn, stop_condition);
							break;*/
						case 7:
							loop = false;
							break;
						}
					}
					system("cls");
					break;
				case 3:
					tabuSearch->setParameters(iterations, not_change, div_not_change, alg_time, num_of_candidates, tabu_length, diversificationOn, stop_condition);
					performanceCountStart = startTimer();
					solution = tabuSearch->algorithm();
					performanceCountEnd = endTimer();
					time = (double)(performanceCountEnd.QuadPart - performanceCountStart.QuadPart);
					duration(time, 1);
					cout << "MIN: " << solution;
					cout << "\nCzas trwania algorytmu: " << time << " [ms]" << endl;
					cout << endl << "Nacisnij dowolny klawisz aby kontynuowac...";
					cin.ignore();
					cin.get();
					break;
				case 4:
					loop2 = false;
					break;
				default:
					break;
				}
			}
			break;
		case 2:
			testTabuAtsp();
			break;
		case 3:
			loop3 = false;
			break;
		default:
			break;
		}
	}
};

void simulatedMenu(string filename)
{
	LARGE_INTEGER performanceCountStart, performanceCountEnd;
	double alphas[5] = { 0.9, 0.95, 0.99, 0.995, 0.999 };
	int period = 50;
	double Temperature = 100;
	bool autTemp = true;
	double endTemperature = 0.02;
	double ile = 10.0;

	string menu = "|.........................................................................|\n"
		"|.......... ________________________________ .............................|\n"
		"|..........|      _  _   _                  |.............................|\n"
		"|..........|     / |/ | |_ |\\ | | |         |.............................|\n"
		"|..........|    /     | |_ | \\| |_|         |.............................|\n"
		"|..._______|________________________________|____________________.........|\n"
		"|..|                   Simulated Annealing                       |........|\n"
		"|..|     OPCJA                                    KLAWISZ        |........|\n"
		"|..|                                                             |........|\n"
		"|..|   1. POJEDYNCZE ROZWIAZANIE                     1           |........|\n"
		"|..|   2. PRZEPROWADZ TESTY                          2           |........|\n"
		"|..|   3. COFNIJ                                     3           |........|\n"
		"|..|_____________________________________________________________|........|\n"
		"|.........................................................................|\n"
		"|.........................................................................|\n"
		"|.........................................................................|\n"
		"|.........................................................................|\n";

	int option;
	bool loop = true;
	bool loop2 = true;
	bool loop3 = true;
	while (loop3){
		system("cls");
		cout << menu;
		cout << "Wczytany plik: " << filename << endl;
		cin >> option;
		int per = 50;
		int solution = 0;
		double tk = 0.2;
		double alpha = 0.999;
		double czas = 0;
		double startTemp = 0;
		switch (option){
		case 1://menu1
			loop2 = true;
			while (loop2){
				system("cls");
				cout << "Parametr s: " << per << endl << "Parametr alpha: " << alpha << endl << "Temperatura koncowa: " << tk << endl << endl;
				cout << "1. Wyswietl macierz sasiedztwa\n2. Edytuj parametry\n3. Rozpocznij alogrytm\n4. Powrot\n";
				cin >> option;
				switch (option)
				{
				case 1:
					cout << endl;
					simulatedAnnealing->getMacierz()->show_matrix();
					cout << endl << "Nacisnij dowolny klawisz aby kontynuowac...";
					cin.ignore();
					cin.get();
					break;
				case 2:
					cout << "\nPodaj s:\n";
					cin >> per;
					cout << "\nPodaj alpha:\n";
					cin >> alpha;
					cout << "\nPodaj temperature koncowa\n";
					cin >> tk;
					system("cls");
					break;
				case 3:
					cout << "MIN: ";
					performanceCountStart = startTimer();
					solution = simulatedAnnealing->algorithm(per, alpha, startTemp, tk);
					performanceCountEnd = endTimer();
					czas = (double)(performanceCountEnd.QuadPart - performanceCountStart.QuadPart);
					duration(czas, 1);
					cout << solution << "\nCzas trwania algorytmu: " << czas << " [ms]" << endl;
					cout << "Temperatura pocz�tkowa: " << startTemp;
					cout << endl << "Nacisnij dowolny klawisz aby kontynuowac...";
					cin.ignore();
					cin.get();
					break;
				case 4:
					loop2 = false;
					break;
				default:
					break;
				}
			}
			break;
		case 2:
			if (filename == "brak pliku"){
				system("cls");
				cout << "Nie wybrano pliku";
				cin.ignore();
				cin.get();
				break;
			}
			loop = true;
			while (loop){
				system("cls");
				cout << "Aktualne parametry alpha: ";
				for (int i = 0; i < 5; i++)
					cout << alphas[i] << "  ";
				cout << "\nAktualne parametry s: n , (n^2)/2, " << period << endl;
				cout << "Temperatura poczatkowa: ";
				if (autTemp)cout << "automatycznie\n";
				else cout << Temperature;
				cout << "Temperatura koncowa: " << endTemperature << endl;
				cout << "Ilosc powtorzen algorytmu: " << ile << endl << endl;
				cout << "1. Edytuj parametry alpha\n2. Edytuj parametr s\n3. Edytuj temperature poczatkowa\n4. Edytuj temperature koncowa\n5. Edytuj liczbe powtorzen\n6. Rozpocznij test algorytmu z powyzszymi parametrami\n7. Powrot\n";
				cin >> option;
				switch (option)
				{
				case 1:
					system("cls");
					for (int i = 0; i < 5; i++){
						cout << "Podaj parametra alpha \n" << i << endl;
						cin >> alphas[i];
						cout << endl;
					}
					break;
				case 2:
					system("cls");
					cout << "Podaj parametr s\n";
					cin >> period;
					break;
				case 3: break;
				case 4:
					system("cls");
					cout << "Podaj temperature koncowa\n";
					cin >> endTemperature;
					break;
				case 5:
					system("cls");
					cout << "Podaj liczbe powtorzen\n";
					cin >> ile;
					break;
				case 6:
					system("cls");
					testAnnealing(filename, period, alphas, ile, endTemperature);
					cin.ignore();
					cin.get();
				case 7:
					loop = false;
					break;
				default: break;
				}
			}
			break;
		case 3:
			loop3 = false;
			break;
		default:
			break;
		}
	}
};

void testAnnealing(string filename, int period, double  alphas[], double ile, double endTemperature){
	int s;
	LARGE_INTEGER performanceCountStart, performanceCountEnd;
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	ofstream plik;
	//filename.find('.');
	plik.open("wyniki " + filename + ".txt");
	plik << filename << endl << "PERIOD ALPHA T0 TIME MINIMUM" << endl;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			switch (j)
			{
			case 0: s = period; break;
			case 1: s = simulatedAnnealing->getSize(); break;
			case 2: s = (simulatedAnnealing->getSize()*simulatedAnnealing->getSize()) / 2; break;
			default:
				break;
			}

			double time = 0;
			double t0 = 0;
			int minimum = 0;
			for (int k = 0; k < ile; k++)
			{
				performanceCountStart = startTimer();
				minimum += simulatedAnnealing->algorithm(s, alphas[i], t0, endTemperature);
				performanceCountEnd = endTimer();
				time += (performanceCountEnd.QuadPart - performanceCountStart.QuadPart);
			}
			time = time / freq.QuadPart * 1000;
			plik << s << " ";// << " ALPHA ";
			plik << alphas[i] << " ";//
			plik << t0 / ile << " ";
			plik << time / ile << " ";
			plik << minimum / ile;
			plik << endl;
			cout << endl << "----------------------------------------------";
			cout << endl << "EPOKA " << s << " ALPHA " << alphas[i] << " T0 " << t0 / ile;
			cout << " TIME " << time / ile;
			cout << endl << " MINIMUM " << minimum / ile;

		}
	}
};

void testGenetic()
{
	LARGE_INTEGER performanceCountStart, performanceCountEnd;
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	ofstream resultFile;

	ostringstream geneticTestMenuStream;
	string geneticTestMenu;
	string dataFilesStr;
	string bestKnownSolutionsStr;
	string testScenarioStr;
	string valuesOfTestedFactorStr;
	string resultFileStr = getCurrentTime() + ".csv" ;

	int numOfInstances = 7;
	// nazwy plik�w z instancjami
	string* dataFiles = new string[numOfInstances]();			
	dataFiles[0] = "br17.atsp";
	dataFiles[1] = "ftv47.atsp";
	dataFiles[2] = "ftv64.atsp";
	dataFiles[3] = "kro124p.atsp";
	dataFiles[4] = "ftv170.atsp";
	dataFiles[5] = "rbg323.atsp";
	dataFiles[6] = "rbg443.atsp";

	//najlepsze znane rozwiazania dla powyzszych instancji
	int *bestKnownSolutions = new int[numOfInstances]();			
	bestKnownSolutions[0] = 39;
	bestKnownSolutions[1] = 1776;
	bestKnownSolutions[2] = 1839;
	bestKnownSolutions[3] = 36230;
	bestKnownSolutions[4] = 2755;
	bestKnownSolutions[5] = 1326;
	bestKnownSolutions[6] = 2720;

	int numOfTestedValues = 6;
	double *valuesOfTestedFactor = new double[numOfTestedValues]();
	valuesOfTestedFactor[0] = 200;
	valuesOfTestedFactor[1] = 100;
	valuesOfTestedFactor[2] = 50;
	valuesOfTestedFactor[3] = 25;
	valuesOfTestedFactor[4] = 10;
	valuesOfTestedFactor[5] = 5;

	// testy wg okreslonego parametru 0 - liczba osobnikow w populacji, 1 - ilosc populacji, 2 - ilosc mutowanych genow, 3 - prawdopodobienstwo mutacji, 4 - ilosc potomkow
	int testScenario = 3;							
	// ile powtorzen
	int numOfIterationsPerSingleRun = 2;											
	// zmienna na rozwiazanie
	int solution = 0;

	char choice;
	bool goBack = false;
	bool showMenu = true;
	double time = 0;
	double percentangeError = 0;
	int newValue;
	string newStringValue;

	string possibleTestScenario[5] = { "Size of population", "Population quantity", "Quantity of mutating genes", "Probability of mutation", "Children quantity" };

	while (!goBack){
		dataFilesStr = arrayToString(dataFiles, numOfInstances);
		bestKnownSolutionsStr = arrayToString(bestKnownSolutions, numOfInstances);
		testScenarioStr = possibleTestScenario[testScenario];
		valuesOfTestedFactorStr = arrayToString(valuesOfTestedFactor, numOfTestedValues);

		geneticTestMenuStream.str("");
		geneticTestMenuStream.clear();
		geneticTestMenuStream <<
			"|.........................................................................|\n"
			"|.......... ________________________________ .............................|\n"
			"|..........|      _  _   _                  |.............................|\n"
			"|..........|     / |/ | |_ |\\ | | |         |.............................|\n"
			"|..........|    /     | |_ | \\| |_|         |.............................|\n"
			"|..._______|________________________________|____________________.........|\n"
			"|..|                                                             |........|\n"
			"|..|                   GENETIC ALGORITHM                         |........|\n"
			"|..|     OPTION                                     KEY          |........|\n"
			"|..|                                                             |........|\n"
			"|..|	  START TEST                                 S           |........|\n"
			"|..|      CHANGE VALUE OF PARAMETER NR. X            X           |........|\n"
			"|..|      BACK                                       B           |........|\n"
			"|..|                                                             |........|\n"
			"|..|_____________   PARAMETERS OF TEST   ________________________|........|\n"
			"|..|                                                             |........|\n"
			"|..|   1. NUMBER OF INSTANCES                      " << myfillandw(' ', 3) << numOfInstances << "           |........|\n"
			"|..|      DATA FILES [*.atsp]                                    |........|\n"
			"|..|      "<< myfillandw(' ', 49)<< dataFilesStr <<"      |........|\n"
			"|..|                                                             |........|\n"
			"|..|   2. BEST KNOWN SOLUTIONS                                   |........|\n"
			"|..|      " << myfillandw(' ', 49) << bestKnownSolutionsStr <<"      |........|\n"
			"|..|                                                             |........|\n"
			"|..|   3. TESTED FACTOR                                          |........|\n"
			"|..|                  " << myfillandw(' ', 26) << testScenarioStr << "                 |........|\n"   
			"|..|                                                             |........|\n"             
			"|..|   4. VALUES OF TESTED FACTOR                                |........|\n"
			"|..|                  " << myfillandw(' ', 26) << valuesOfTestedFactorStr << "                 |........|\n"
			"|..|                                                             |........|\n"            
			"|..|   5. ITERATIONS PER SCENARIO                  " << myfillandw(' ', 3) << numOfIterationsPerSingleRun << "           |........|\n"
			"|..|                                                             |........|\n"
			"|..|   6. NAME OF RESULT FILE                                    |........|\n"
			"|..|                  " << myfillandw(' ', 26) << resultFileStr << "                 |........|\n"
			"|..|_____________________________________________________________|........|\n"
			"|.........................................................................|\n"
			"|.........................................................................|\n"
			"|.........................................................................|\n"
			"|.........................................................................|\n";
		geneticTestMenu = geneticTestMenuStream.str();
		time = 0;
		if (showMenu) {
			system("cls");
			cout << geneticTestMenu;
		}
		printConsole();
		cin >> choice;

		switch (choice) {
			case '1':
				cout << "Input new value for instances quantity (>0): ";
				cin >> newValue;
				if (newValue <= 0)
					break;
				numOfInstances = newValue;
				cout << "Provide names of data files (.atsp) : ";
				for (int i = 1; i<=numOfInstances; i++)
				{
					cout << endl << i << " of " << numOfInstances << " : ";
					cin >> newStringValue;
					if (newStringValue.find(".atsp") != string::npos) {
					}
					else {
						newStringValue += ".atsp";
					}

					if (!fileExists(newStringValue)) {
						cout << endl << "There is no such file in directory, try again.";
						i--;
					}
					else {
						dataFiles[i - 1] = newStringValue;
					}
				}
			case '2':
				cout << "Provide best known solutions for these instances." << endl;
				for (int i = 1; i<=numOfInstances; i++)
				{
					cout << endl << i << " of " << numOfInstances << " : ";
					cin >> bestKnownSolutions[i-1];
				}
				break;
			case '3':
				cout << "Which factor you want to test? \n\t0 - size of population\n\t1 - quantity of population\n\t2 - quantity of mutating genes\n\t3 - probability of mutation\n\t4 - children quantity\nYour choice : ";
				cin >> newValue;
				if (newValue >= 0 && newValue <= 4) testScenario = newValue;
				break;
			case '4':
				cout << "How many values do you want to test? ";
				cin >> numOfTestedValues;
				cout << "Provide values of tested factor \n ";
				for (int i = 1; i<=numOfTestedValues; i++)
				{
					cout << endl << i << " of " << numOfInstances << " : ";
					cin >> valuesOfTestedFactor[i-1];
				}
				break;
			case '5':
				cout << "Input new value for iteration per one scenario (>0): ";
				cin >> newValue;
				if (newValue>0) 
					numOfIterationsPerSingleRun = newValue;
				break;
			case '6':
				do{
					cout << endl << "Input name of result file (.txt): ";
					cin >> newStringValue;
				} while (resultFileStr.empty());

				if (newStringValue.find(".csv") != string::npos) {
				}
				else {
					newStringValue += ".csv";
				}
				if (fileExists(newStringValue)) {
					cout << "There is a file with this name. Are you sure ? (y/n) \n : ";
					cin >> choice;
					if (choice == 'y')
						resultFileStr = newStringValue;
				}
				else {
					resultFileStr = newStringValue;
				}
				break;
			case 's':
			case 'S':
				resultFile.open(resultFileStr.c_str());
				if (!resultFile.good()){
					cout << "\nError with creating file.";
				} else {
					cout << "Test started. Please wait, it may take a while..." << endl;
					cout << "TESTED FACTOR: " << testScenarioStr << endl;
					cout << "Number of runs per single scenario : " << numOfIterationsPerSingleRun << endl;
					resultFile << "TESTED FACTOR: " << testScenarioStr << "\n";
					resultFile << "Number of runs per single scenario : " << numOfIterationsPerSingleRun << "It means that every measurement is average of " << numOfIterationsPerSingleRun << " runs" << "\n";
					for (int i = 0; i<numOfInstances; i++) {

						cout << "Test for instance: " << dataFiles[i] << "\nLoading...\n";
						resultFile << "," << dataFiles[i] << endl;
						resultFile << ",,Value of factor, time[ms], solution, percentage error/n";
						
						delete geneticAlgorithm;
						geneticAlgorithm = new GeneticAlgorithm(dataFiles[i]);

						for (int k = 0; k<numOfTestedValues; k++)
						{
							cout << endl << "\t" << k+1 << " value of " << numOfTestedValues << " : " << valuesOfTestedFactor[k] << endl;
							resultFile << ",," << valuesOfTestedFactor[k];
							time = 0;
							solution = 0;
							
							switch (testScenario) {
								case 0:
									geneticAlgorithm->sizeOfPopulation = (int)valuesOfTestedFactor[k];
									geneticAlgorithm->numberOfChild = (int)valuesOfTestedFactor[k] / 2;
									break;
								case 1:
									geneticAlgorithm->numberOfPopulation = (int)valuesOfTestedFactor[k];
									break;
								case 2:
									geneticAlgorithm->numberOfGenes = (int)valuesOfTestedFactor[k];
									break;
								case 3:
									geneticAlgorithm->numberOfGenes = (int)(geneticAlgorithm->getSize() * 0.1);
									geneticAlgorithm->probability = (int)valuesOfTestedFactor[k];
									break;
								case 4:
									geneticAlgorithm->numberOfGenes = (int)(geneticAlgorithm->getSize() * valuesOfTestedFactor[k]);
									break;
							}

							for (int j = 1; j <= numOfIterationsPerSingleRun; j++)
							{
								cout << "\t\tIteration " << j << " of " << numOfIterationsPerSingleRun << endl;
								performanceCountStart = startTimer();
								solution += geneticAlgorithm->algorithm();
								performanceCountEnd = endTimer();
								time += (performanceCountEnd.QuadPart - performanceCountStart.QuadPart);
							}

							time /= numOfIterationsPerSingleRun;
							solution /= numOfIterationsPerSingleRun;
							time = time / freq.QuadPart * 1000;
							
							resultFile << "," << time;
							cout << "Mean time : " << time << endl;
							
							resultFile << "," << solution;
							cout << "Mean solution : " << solution << endl;
							
							percentangeError = (solution / (bestKnownSolutions[i] * 1.0) - 1) * 100;
							cout << "Mean percentage error = " << percentangeError << endl;
							resultFile << "," << percentangeError << "\n";
						}
					}
					resultFile.close();
					cout << endl << "Press any key to continue...";
					cin.ignore();
					cin.get();
				}
				break;
			case 'b':
			case 'B':
				goBack = true;
				break;
			default:
				break;
		}
	}
}

void testTabuAtsp()
{
	int ile_instancji = 7;

	string* files = new string[ile_instancji]();			// nazwy plik�w z instancjami
	files[0] = "br17.atsp";
	files[1] = "ftv47.atsp";
	files[2] = "ftv64.atsp";
	files[3] = "kro124p.atsp";
	files[4] = "ftv170.atsp";
	files[5] = "rbg323.atsp";
	files[6] = "rbg443.atsp";

	int *bestSolutions = new int[ile_instancji]();			//najlepsze znane rozwiazania dla powyzszych instancji
	bestSolutions[0] = 39;
	bestSolutions[1] = 1776;
	bestSolutions[2] = 1839;
	bestSolutions[3] = 36230;
	bestSolutions[4] = 2755;
	bestSolutions[5] = 1326;
	bestSolutions[6] = 2720;

	// kryteria stopu - domyslnie ustawiane na czasy, rownie dobrze mozna ustawic liczbe iteracji bez zmiany rozwiazania lub liczbe iteracji
	int how_many_stops = 10;
	double *stopCriteria = new double[how_many_stops]();
	stopCriteria[0] = 100;
	stopCriteria[1] = 300;
	stopCriteria[2] = 600;
	stopCriteria[3] = 800;
	stopCriteria[4] = 1500;
	stopCriteria[5] = 3000;
	stopCriteria[6] = 6000;
	stopCriteria[7] = 10000;
	stopCriteria[8] = 15000;
	stopCriteria[9] = 20000;

	int repeat = 2;											// ile powtorzen
	string file_result = "l_kandydatow.txt";

	// PARAMETRY ALGORYTMU


	int size = tabuSearch->getSize();
	//int iterations = 50000;						// liczba iteracji petli glownej algorytmu
	//int not_change = size*4;					// maksymalna liczba iteracji bez poprawy rozwiazania
	int div_not_change = 20;						// mnozone razy size-  max liczba iteracji bez poprawy rozwiazania do zastosowania dywersyfikacji
	//double alg_time = 1;						// czas dzialania algorytmu
	int num_of_candidates = 2;					// mnozone razy size - liczba kandydat�w 
	int tabu_length = 10;						// dlugosc listy tabu
	//bool diversificationOn = true;				// 0 - wylaczona 1 - wlaczona 
	int stop_condition = 0;						// 0 - interacje, 1 - time, 2 - not_change

	int solution = 0;							// zmienna na rozwiazanie

	string menu = "|.........................................................................|\n"
		"|.......... ________________________________ .............................|\n"
		"|..........|      _  _   _                  |.............................|\n"
		"|..........|     / |/ | |_ |\\ | | |         |.............................|\n"
		"|..........|    /     | |_ | \\| |_|         |.............................|\n"
		"|..._______|________________________________|____________________.........|\n"
		"|..|                   Tabu Search                               |........|\n"
		"|..|     OPCJA                                    KLAWISZ        |........|\n"
		"|..|                                                             |........|\n"
		"|..|   1. ZOBACZ I USTAW PARAMETRY ALGORYTMU         1           |........|\n"
		"|..|   2. ZOBACZ I USTAW PARAMETRY TESTU             2           |........|\n"
		"|..|   3. PRZEPROWADZ TEST                           3           |........|\n"
		"|..|                                                             |........|\n"
		"|..|   4. COFNIJ                                     4           |........|\n"
		"|..|_____________________________________________________________|........|\n"
		"|.........................................................................|\n"
		"|.........................................................................|\n"
		"|.........................................................................|\n"
		"|.........................................................................|\n";

	int option;
	bool loop = true;
	bool loop2 = true;
	bool loop3 = true;
	double time = 0;
	double percent = 0;

	LARGE_INTEGER performanceCountStart, performanceCountEnd;
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	ofstream file;


	while (loop3){
		system("cls");
		cout << menu;
		cin >> option;
		switch (option)
		{
		case 1:
			loop = true;
			while (loop)
			{
				system("cls");
				cout << "\n --- Aktualne parametry algorytmu ---\n\n";
				cout << "SIZE : " << size << endl;
				cout << "Warunek stopu(1): ";
				if (stop_condition == 0) cout << " Liczba iteracji\n";
				else if (stop_condition == 1) cout << " Czas\n";
				else if (stop_condition == 2) cout << " Brak zmiany rozwiazania\n";
				cout << "Mnoznik parametru not_change dla dywersyfikacji(2): " << div_not_change << endl;
				cout << "Dlugosc listy tabu(3) : " << tabu_length << "\n";
				cout << "Mnoznik liczby kandydatow(4) : " << num_of_candidates << endl;
				cout << "\nCofnij(5)\nOpcja nr: ";
				int ktory;
				cin >> ktory;
				cout << "\n";
				switch (ktory)
				{
				case 1:
					cout << "Podaj odpowiedni warunek stopu : \n 0 - interacje, 1 - czas, 2 - brak zmiany rozwiazania\n";
					cin >> ktory;
					if (ktory >= 0 && ktory <3) stop_condition = ktory;
					break;
				case 2:
					cout << "Podaj liczbe przez jaka ma byc mnozony rozmiar(np.not_change=2*size) : ";
					cin >> ktory;
					if (ktory>0) div_not_change = ktory;
					break;
				case 3:
					do{
						cout << "Podaj dlugosc listy tabu : ";
						cin >> ktory;
					} while (ktory <= 0);
					tabu_length = ktory;
					break;
				case 4:
					cout << "Podaj liczbe przez jaka ma byc mnozony rozmiar(np.l_kandydatow = 2*size) : ";
					cin >> ktory;
					if (ktory>1) num_of_candidates = ktory;
					break;
				case 5:
					loop = false;
					break;
				}
			}
			system("cls");
			break;
		case 2:
			loop = true;
			while (loop)
			{
				system("cls");
				cout << "\n --- Aktualne parametry dla testow ---\n\n";
				cout << "Testowany algorytm : Tabu Search\n";
				cout << "Ilosc instancji(1) : " << ile_instancji << endl;
				cout << "Nazwy plikow(.atsp) z miastami(1) : {";
				for (int i = 0; i<ile_instancji; i++)
				{
					cout << files[i];
					if (i != ile_instancji - 1)cout << " , ";
				}
				cout << "}" << endl;
				cout << "Najlepsze znane rozwiazania dla tych instancji(2) : {";
				for (int i = 0; i<ile_instancji; i++)
				{
					cout << bestSolutions[i];
					if (i != ile_instancji - 1)cout << " , ";
				}
				cout << "}" << endl;
				cout << "Ile i jakie kryteria zatrzymania algorytmu";
				if (stop_condition == 0) cout << " (liczby iteracji)";
				else if (stop_condition == 1) cout << " (czasy)";
				else if (stop_condition == 2) cout << " (brak zmiany rozwiazania)";
				cout << "(3) : {";
				for (int i = 0; i<how_many_stops; i++)
				{
					cout << stopCriteria[i];
					if (i != how_many_stops - 1)cout << " , ";
				}
				cout << "}" << endl;
				cout << "Ilosc powtorzen(4) : " << repeat;
				cout << "\nNazwa pliku z wynikami(5) : " << file_result;
				cout << "\nCofnij(6)\nOpcja nr: ";

				int ktory;
				cin >> ktory;
				cout << "\n";
				switch (ktory)
				{
				case 1:
					do{
						cout << "Podaj ilosc instancji (>0) : \n";
						cin >> ile_instancji;
					} while (ile_instancji <= 0);
					cout << "Podaj nazwy plikow z miastami (.atsp) : \n";
					for (int i = 0; i<ile_instancji; i++)
					{
						cout << i << " : ";
						cin >> files[i];
						cout << endl;
					}
				case 2:
					cout << "Podaj najlepsze znane rozwiazania dla tych plikow\n";
					for (int i = 0; i<ile_instancji; i++)
					{
						cout << i << " : ";
						cin >> bestSolutions[i];
						cout << endl;
					}
					break;
				case 3:
					cout << "Podaj ilosc kryteriow zatrzymania : ";
					cin >> how_many_stops;
					cout << "Podaj wartosc kryterium ";
					if (stop_condition == 0) cout << " (liczby iteracji)";
					else if (stop_condition == 1) cout << " (czasy)";
					else if (stop_condition == 2) cout << " (brak zmiany rozwiazania)";
					cout << "\n";
					for (int i = 0; i<how_many_stops; i++)
					{
						cout << i << " : ";
						cin >> stopCriteria[i];
						cout << endl;
					}
					break;
				case 4:
					do{
						cout << "Podaj ilosc powtorzen (>0) : ";
						cin >> repeat;
					} while (repeat <= 0);
					break;
				case 5:
					do{
						cout << "Podaj nazwe pliku z rozszerzeniem .txt : ";
						cin >> file_result;
					} while (file_result.empty());
					break;
				case 6:
					loop = false;
					break;
				}
			}
			break;
		case 3:
			file.open(file_result.c_str());
			if (file.good()){
				for (int i = 0; i<ile_instancji; i++)
				{
					cout << "Test dla pliku : " << files[i] << "\nWczytywanie...\n";
					file << files[i] << endl;
					delete tabuSearch;
					tabuSearch = new TabuSearch(files[i]);
					size = tabuSearch->getSize();
					cout << "Kryterium stopu : ";
					file << "Kryterium stopu : ";
					if (stop_condition == 0) cout << " - liczba iteracji.\n";
					else if (stop_condition == 1) cout << " - czas.\n";
					else if (stop_condition == 2) cout << " - brak zmiany rozwiazania.\n";
					if (stop_condition == 0) file << " - liczba iteracji.\n";
					else if (stop_condition == 1) file << " - czas.\n";
					else if (stop_condition == 2) file << " - brak zmiany rozwiazania.\n";
					file << "WITH DIVERSIFICATION" << endl;
					for (int k = 0; k<how_many_stops; k++)
					{
						cout << "STOP AT: " << stopCriteria[k] << endl;
						file.width(12);
						file << stopCriteria[k];
						time = 0;
						solution = 0;
						if (stop_condition == 0)
							tabuSearch->setParameters((int)(stopCriteria[k]), -1, div_not_change*size, -1, num_of_candidates*size, tabu_length, true, 0);
						else if (stop_condition == 1)
							tabuSearch->setParameters(-1, -1, div_not_change*size, (int)(stopCriteria[k]), num_of_candidates*size, tabu_length, true, 1);
						else if (stop_condition == 2)
							tabuSearch->setParameters(-1, (int)(stopCriteria[k]), div_not_change*size, -1, num_of_candidates*size, tabu_length, true, 2);
						for (int j = 1; j <= repeat; j++)
						{
							cout << "REPEAT with diversification : " << j << endl;
							performanceCountStart = startTimer();
							solution += tabuSearch->algorithm();
							performanceCountEnd = endTimer();
							time += (performanceCountEnd.QuadPart - performanceCountStart.QuadPart);
						}
						time /= repeat;
						solution /= repeat;
						time = time / freq.QuadPart * 1000;
						if (stop_condition != 1)
						{
							file.width(13);
							file << time;
						}
						cout << "Sredni czas = " << time << endl;
						file.width(13);
						file << solution;
						cout << "Sredni wynik = " << solution << endl;
						percent = (solution / (bestSolutions[i] * 1.0) - 1) * 100;
						cout << "Procent bledu = " << percent << endl;
						file.width(13);
						file << percent << endl;
					}
					file << "WITHOUT DIVERSIFICATION" << endl;
					for (int k = 0; k<how_many_stops; k++)
					{
						cout << "STOP AT: " << stopCriteria[k] << endl;
						file.width(12);
						file << stopCriteria[k];
						time = 0;
						solution = 0;
						if (stop_condition == 0)
							tabuSearch->setParameters((int)stopCriteria[k], -1, div_not_change*size, -1, num_of_candidates*size, tabu_length, false, 0);
						else if (stop_condition == 1)
							tabuSearch->setParameters(-1, -1, div_not_change*size, (int)stopCriteria[k], num_of_candidates*size, tabu_length, false, 1);
						else if (stop_condition == 2)
							tabuSearch->setParameters(-1, (int)stopCriteria[k], div_not_change*size, -1, num_of_candidates*size, tabu_length, false, 2);
						for (int j = 1; j <= repeat; j++)
						{
							cout << "REPEAT without diversification : " << j << endl;
							performanceCountStart = startTimer();
							solution += tabuSearch->algorithm();
							performanceCountEnd = endTimer();
							time += (performanceCountEnd.QuadPart - performanceCountStart.QuadPart);
						}
						time /= repeat;
						solution /= repeat;
						time = time / freq.QuadPart * 1000;
						if (stop_condition != 1)
						{
							file.width(13);
							file << time;
						}
						cout << "Sredni czas = " << time << endl;
						file.width(13);
						file << solution;
						cout << "Sredni wynik = " << solution << endl;
						percent = (solution / (bestSolutions[i] * 1.0) - 1) * 100;
						cout << "Procent bledu = " << percent << endl;
						file.width(13);
						file << percent << endl;
					}

				}
				cout << endl << "Nacisnij dowolny klawisz aby kontynuowac...";
				cin.ignore();
				cin.get();
				file.close();
			}
			else cout << "\nProblem z plikiem wyjsciowym";
			break;
		case 4:
			loop3 = false;
			break;
		default:
			break;
		}
	}
}
