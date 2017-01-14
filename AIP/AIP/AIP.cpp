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

using namespace std;

//FUNKCJE DO POMIARU CZASU
LARGE_INTEGER startTimer()
{
	LARGE_INTEGER start;
	DWORD_PTR oldmask = SetThreadAffinityMask(GetCurrentThread(), 0);
	QueryPerformanceCounter(&start);
	SetThreadAffinityMask(GetCurrentThread(), oldmask);
	return start;
}
LARGE_INTEGER endTimer()
{
	LARGE_INTEGER stop;
	DWORD_PTR oldmask = SetThreadAffinityMask(GetCurrentThread(), 0);
	QueryPerformanceCounter(&stop);
	SetThreadAffinityMask(GetCurrentThread(), oldmask);
	return stop;
}

//Funkcja obliczajaca czas w [s]
double duration(double &time, int repeat){
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	time = time / freq.QuadPart;
	time = time / repeat;
	return time;
}

//Funkcja generuj¹ca plik z przyk³adow¹ instancj¹
//void generuj_plik(string,int,int,int);
void testGenetic();
void testAnnealing(string filename, int period, double  alphas[], double ile, double endTemperature);
void testTabuAtsp();
void geneticMenu(string);
void simulatedMenu(string);
void tabuMenu(string);

string menu = "|.........................................................................|\n"
"|.......... ________________________________ .............................|\n"
"|..._______|________PROBLEM KOMIWOJAZERA____|____________________.........|\n"
"|..|                                                             |........|\n"
"|..|   .....ALGORYTMY......                                      |........|\n"
"|..|                                                             |........|\n"
"|..|   1.SYMULOWANE WYZARZANIE                       1           |........|\n"
"|..|   2.METODA TABU SEARCH                          2           |........|\n"
"|..|   3.ALGORYTM GENETYCZNY                         3           |........|\n"
"|..|                                                             |........|\n"
"|..|   ----- OPCJE ------                                        |........|\n"
"|..|   4.WCZYTAJ Z PLIKU                             4           |........|\n"
"|..|   5.WYSWIETL WCZYTANA MACIERZ                   5           |........|\n"
"|..|                                                             |........|\n"
"|..|                                                             |........|\n"
"|..|   7.KONIEC                                      k           |........|\n"
"|..|________________________AUTORZY______________________________|........|\n"
"|..|                                                             |........|\n"
"|..|   MICHAL SZTUKA                           200798            |........|\n"
"|..|   JOANNA TUROWSKA                         236088            |........|\n"
"|..|_____________________________________________________________|........|\n"
"|.........................................................................|\n"
"|.........................................................................|\n";

//funkcja zamieniaj¹ca liczbê na string
string ltos(int);

Tabu_search* tabu_search;
SimulatedAnnealing* simulated_Annealing;
GeneticAlgorithm* genetic_Algorithm;

int _tmain(int argc, _TCHAR* argv[])
{
	srand(time(NULL));
	LARGE_INTEGER performanceCountStart, performanceCountEnd;
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);

	int licznik = 0;
	double time = 0;

	char wybor;
	string plik;
	string loaded = "br17.atsp";
	simulated_Annealing = new SimulatedAnnealing(loaded);
	tabu_search = new Tabu_search(loaded);
	genetic_Algorithm = new GeneticAlgorithm(loaded);
	while (1)
	{
		if (licznik == 0 && (genetic_Algorithm->getSize() > 0))
		{
			system("cls");
			cout << menu << "\nWczytano dane z pliku: " << loaded;
		}
		else if (licznik == 0) {
			system("cls");
			cout << menu << "\nNie udalo sie wczytac domyslnego pliku";
		}
		licznik++;
		cout << "\nconsole > ";
		cin >> wybor;
		switch (wybor)
		{
		case '1':
			if (loaded == "brak pliku")
			{
				cout << "\nconsole> Nie wczytano pliku.";
				break;
			}
			simulatedMenu(loaded);
			licznik = 0;
			break;
		case '2':
			if (loaded == "brak pliku")
			{
				cout << "\nconsole> Nie wczytano pliku.";
				break;
			}
			tabuMenu(loaded);
			licznik = 0;
			break;
		case '3':
			if (loaded == "brak pliku")
			{
				cout << "\nconsole> Nie wczytano pliku.";
				break;
			}
			geneticMenu(loaded);
			licznik = 0;
			break;
		case '4':
			loaded = "";
			cin >> loaded;
			if (loaded.find(".atsp") != string::npos)
			{
				delete simulated_Annealing;
				delete tabu_search;
				delete genetic_Algorithm;
				simulated_Annealing = new SimulatedAnnealing(loaded);
				tabu_search = new Tabu_search(loaded);
				genetic_Algorithm = new GeneticAlgorithm(loaded);
			}
			else cout << "\nconsole> Nie udalo sie wczytac pliku. Podaj nazwe pliku z koncowka .atsp";
			break;
		case '5':
			cout << endl;
			tabu_search->showMatrix();
			cout << endl << "Nacisnij dowolny klawisz aby kontynuowac...";
			cin.ignore();
			cin.get();
			break;
		case 'k':
			exit(0);
		}
	}

	system("PAUSE");
	return 0;
}
void geneticMenu(string filename)
{
	LARGE_INTEGER performanceCountStart, performanceCountEnd;
	LARGE_INTEGER freq;

	string menu = "|.........................................................................|\n"
		"|.......... ________________________________ .............................|\n"
		"|..........|      _  _   _                  |.............................|\n"
		"|..........|     / |/ | |_ |\\ | | |         |.............................|\n"
		"|..........|    /     | |_ | \\| |_|         |.............................|\n"
		"|..._______|________________________________|____________________.........|\n"
		"|..|                   Genetic Algorithm                         |........|\n"
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

	int size = genetic_Algorithm->getSize();
	int sizeOfPopulation = 100;					// Rozmiar populacji
	int numberOfPopulation = 100;				// Liczba populacji
	int numberOfGenes = 1;						// liczba genów ulegajaca mutacji
	int probability = 100;						// prawdopodobienstwo mutacji
	int numberOfChild = 80;						// Ilosc potomkow

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
				cout << "Ilosc osobnikow w populacji: " << sizeOfPopulation << endl;
				cout << "Liczba populacji: " << numberOfPopulation << endl;
				cout << "Prawdopodobienstwo wystapienia mutacji: " << probability << endl;
				cout << "Ilosc genow ulegajacych mutacji: " << numberOfGenes << endl;
				cout << "Ilosc potomkow: " << numberOfChild << "\n";
				cout << "1. Wyswietl macierz sasiedztwa\n2. Edytuj parametry\n3. Rozpocznij alogrytm\n4. Powrot\n";
				cin >> option;
				switch (option)
				{
				case 1:
					cout << endl;
					genetic_Algorithm->showMatrix();
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
						cout << "Ilosc osobnikow w populacji(1): " << sizeOfPopulation << endl;
						cout << "Liczba populacji(2): " << numberOfPopulation << endl;
						cout << "Prawdopodobienstwo wystapienia mutacji(3): " << probability << endl;
						cout << "Ilosc genow ulegajacych mutacji(4): " << numberOfGenes << endl;
						cout << "Ilosc potomkow(5): " << numberOfChild << "\n";
						cout << "\nCofnij(6)\nOpcja nr: ";
						int ktory;
						cin >> ktory;
						cout << "\n";
						switch (ktory)
						{
						case 1:
							cout << "Podaj ilosc osobnikow w populacji: ";
							cin >> ktory;
							if (ktory>0) sizeOfPopulation = ktory;
							break;
						case 2:
							cout << "Podaj liczbe populacji: ";
							cin >> ktory;
							if (ktory>0) numberOfPopulation = ktory;
							break;
						case 3:
							cout << "Podaj prawdopodobienstwo wystapienia mutacji(liczba calkowita): ";
							cin >> ktory;
							if (ktory>0) probability = ktory;
							break;
						case 4:
							cout << "Podaj ilosc genow ulegajacych mutacji: ";
							cin >> ktory;
							if (ktory>0) numberOfGenes = ktory;
							break;
						case 5:
							cout << "Podaj ilosc potomkow: ";
							cin >> ktory;
							if (ktory>0) numberOfChild = ktory;
							break;
						case 6:
							loop = false;
							break;
						}
					}
					system("cls");
					break;
				case 3:
					performanceCountStart = startTimer();
					solution = genetic_Algorithm->algorithm(sizeOfPopulation, numberOfPopulation, numberOfGenes, probability, numberOfChild);
					performanceCountEnd = endTimer();
					time = (performanceCountEnd.QuadPart - performanceCountStart.QuadPart);
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
			testGenetic();
			break;
		case 3:
			loop3 = false;
			break;
		default:
			break;
		}
	}

};
void tabuMenu(string filename)
{
	LARGE_INTEGER performanceCountStart, performanceCountEnd;
	LARGE_INTEGER freq;

	string menu = "|.........................................................................|\n"
		"|.......... ________________________________ .............................|\n"
		"|..........|      _  _   _                  |.............................|\n"
		"|..........|     / |/ | |_ |\\ | | |         |.............................|\n"
		"|..........|    /     | |_ | \\| |_|         |.............................|\n"
		"|..._______|________________________________|____________________.........|\n"
		"|..|                   Tabu Search                               |........|\n"
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

	int size = tabu_search->getSize();
	int iterations = 50000;					// liczba iteracji petli glownej algorytmu
	int not_change = size * 4;					// maksymalna liczba iteracji bez poprawy rozwiazania
	int div_not_change = size * 2;
	double alg_time = 1;				// czas dzialania algorytmu
	int num_of_candidates = size;			// liczba kandydatów 
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
					tabu_search->showMatrix();
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
						cout << "\nZatwierdz zmiany(7)\nCofnij(8)\nOpcja nr: ";
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
						case 7:
							tabu_search->setParameters(iterations, not_change, div_not_change, alg_time, num_of_candidates, tabu_length, diversificationOn, stop_condition);
							break;
						case 8:
							loop = false;
							break;
						}
					}
					system("cls");
					break;
				case 3:
					tabu_search->setParameters(iterations, not_change, div_not_change, alg_time, num_of_candidates, tabu_length, diversificationOn, stop_condition);
					performanceCountStart = startTimer();
					solution = tabu_search->algorithm();
					performanceCountEnd = endTimer();
					time = (performanceCountEnd.QuadPart - performanceCountStart.QuadPart);
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
	LARGE_INTEGER freq;
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
					simulated_Annealing->getMacierz()->show_matrix();
					cout << endl << "Nacisnij dowolny klawisz aby kontynuowac...";
					cin.ignore();
					cin.get();
					break;
				case 2:
					system("cls");
					cout << "Podaj alpha:\n";
					cin >> alpha;
					system("cls");
					cout << "Podaj s:\n";
					cin >> per;
					system("cls");
					cout << "Podaj temperature koncowa\n";
					cin >> tk;
					system("cls");
					break;
				case 3:
					cout << "MIN: ";
					performanceCountStart = startTimer();
					solution = simulated_Annealing->algorithm(per, alpha, startTemp, tk);
					performanceCountEnd = endTimer();
					czas = (performanceCountEnd.QuadPart - performanceCountStart.QuadPart);
					duration(czas, 1);
					cout << solution << "\nCzas trwania algorytmu: " << czas << " [ms]" << endl;
					cout << "Temperatura pocz¹tkowa: " << startTemp;
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
			case 1: s = simulated_Annealing->getSize(); break;
			case 2: s = (simulated_Annealing->getSize()*simulated_Annealing->getSize()) / 2; break;
			default:
				break;
			}

			double time = 0;
			double t0 = 0;
			int minimum = 0;
			for (int k = 0; k < ile; k++)
			{
				performanceCountStart = startTimer();
				minimum += simulated_Annealing->algorithm(s, alphas[i], t0, endTemperature);
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
	int ile_instancji = 7;

	string* files = new string[ile_instancji]();			// nazwy plików z instancjami
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
	int how_many_dif = 6;
	double *arguments = new double[how_many_dif]();
	arguments[0] = 200;
	arguments[1] = 100;
	arguments[2] = 50;
	arguments[3] = 25;
	arguments[4] = 10;
	arguments[5] = 5;
	/*arguments[6] = 4;
	arguments[7] = 10000;
	arguments[8] = 15000;
	arguments[9] = 20000;*/

	int which_test = 3;							// testy wg okreslonego parametru 0 - liczba osobnikow w populacji, 1 - ilosc populacji, 2 - ilosc mutowanych genow, 3 - prawdopodobienstwo mutacji, 4 - ilosc potomkow

	int repeat = 2;											// ile powtorzen
	string file_result = "wg_prawd.txt";

	// PARAMETRY ALGORYTMU


	int size = genetic_Algorithm->getSize();
	int sizeOfPopulation = 50;					// Rozmiar populacji
	int numberOfPopulation = 300;				// Liczba populacji
	int numberOfGenes = 2;						// liczba genów ulegajaca mutacji
	int probability = 50;						// prawdopodobienstwo mutacji
	int numberOfChild = 50;						// Ilosc potomkow

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
				cout << "Ilosc osobnikow w populacji(1): " << sizeOfPopulation << endl;
				cout << "Liczba populacji(2): " << numberOfPopulation << endl;
				cout << "Prawdopodobienstwo wystapienia mutacji(3): " << probability << endl;
				cout << "Ilosc genow ulegajacych mutacji(4): " << numberOfGenes << endl;
				cout << "Ilosc potomkow(5): " << numberOfChild << "\n";
				cout << "\nCofnij(6)\nOpcja nr: ";
				int ktory;
				cin >> ktory;
				cout << "\n";
				switch (ktory)
				{
				case 1:
					cout << "Podaj ilosc osobnikow w populacji: ";
					cin >> ktory;
					if (ktory>0) sizeOfPopulation = ktory;
					break;
				case 2:
					cout << "Podaj liczbe populacji: ";
					cin >> ktory;
					if (ktory>0) numberOfPopulation = ktory;
					break;
				case 3:
					cout << "Podaj prawdopodobienstwo wystapienia mutacji(liczba calkowita): ";
					cin >> ktory;
					if (ktory>0) probability = ktory;
					break;
				case 4:
					cout << "Podaj ilosc genow ulegajacych mutacji: ";
					cin >> ktory;
					if (ktory>0) numberOfGenes = ktory;
					break;
				case 5:
					cout << "Podaj ilosc potomkow: ";
					cin >> ktory;
					if (ktory>0) numberOfChild = ktory;;
					break;
				case 6:
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
				cout << "Testowany algorytm : Algorytm genetyczny\n";
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
				cout << "Testy wedlug(3): ";
				if (which_test == 0) cout << "Ilosc osobnikow w populacji\n";
				else if (which_test == 1) cout << "Liczba populacji\n";
				else if (which_test == 2) cout << "Ilosc mutowanych genow\n";
				else if (which_test == 3) cout << "Prawdopodobienstwo mutacji\n";
				else if (which_test == 4) cout << "Ilosc potomkow\n";
				cout << "Argumenty testow";
				cout << "(4) : {";
				for (int i = 0; i<how_many_dif; i++)
				{
					cout << arguments[i];
					if (i != how_many_dif - 1)cout << " , ";
				}
				cout << "}" << endl;
				cout << "Ilosc powtorzen(5) : " << repeat;
				cout << "\nNazwa pliku z wynikami(6) : " << file_result;
				cout << "\nCofnij(7)\nOpcja nr: ";

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
					cout << "Ktory parametr chcesz testowac\n\t0 - ilosc osobnikow w populacji\n\t1 - liczba populacji\n\t2 - ilosc mutowanych genow\n\t3 - prawdopodobienstwo mutacji\n\t4 - ilosc potomkow\n";
					cin >> ktory;
					if (ktory >= 0 && ktory <= 4) which_test = ktory;
					break;
				case 4:
					cout << "Podaj ilosc instancji : ";
					cin >> how_many_dif;
					cout << "Podaj wartosci liczb osobnikow w populacji\n ";
					for (int i = 0; i<how_many_dif; i++)
					{
						cout << i << " : ";
						cin >> arguments[i];
						cout << endl;
					}
					break;
				case 5:
					cout << "Podaj ilosc powtorzen (>0) : ";
					cin >> ktory;
					if (ktory>0) repeat = ktory;
					break;
				case 6:
					do{
						cout << "Podaj nazwe pliku z rozszerzeniem .txt : ";
						cin >> file_result;
					} while (file_result.empty());
					break;
				case 7:
					loop = false;
					break;
				}
			}
			break;
		case 3:
			file.open(file_result.c_str());
			if (file.good()){
				cout << "TEST WEDLUG: ";
				file << "TEST WEDLUG: ";
				if (which_test == 0)
				{
					cout << "Ilosc osobnikow w populacji\n";
					file << "Ilosc osobnikow w populacji\n";
				}
				else if (which_test == 1)
				{
					cout << "Liczba populacji\n";
					file << "Liczba populacji\n";
				}
				else if (which_test == 2)
				{
					cout << "Ilosc mutowanych genow\n";
					file << "Ilosc mutowanych genow\n";
				}
				else if (which_test == 3)
				{
					cout << "Prawdopodobienstwo mutacji\n";
					file << "Prawdopodobienstwo mutacji\n";
				}
				else if (which_test == 4)
				{
					cout << "Ilosc potomkow\n";
					file << "Ilosc potomkow\n";
				}
				for (int i = 0; i<ile_instancji; i++)
				{
					cout << "Test dla pliku : " << files[i] << "\nWczytywanie...\n";
					file << files[i] << endl;
					delete genetic_Algorithm;
					genetic_Algorithm = new GeneticAlgorithm(files[i]);
					size = genetic_Algorithm->getSize();
					if (which_test == 0)
					{
						cout << "Ilosc osobnikow w populacji\n";
						file << "Ilosc osobnikow w populacji\n";
					}
					else if (which_test == 1)
					{
						cout << "Liczba populacji\n";
						file << "Liczba populacji\n";
					}
					else if (which_test == 2)
					{
						cout << "Ilosc mutowanych genow\n";
						file << "Ilosc mutowanych genow\n";
					}
					else if (which_test == 3)
					{
						cout << "Prawdopodobienstwo mutacji\n";
						file << "Prawdopodobienstwo mutacji\n";
					}
					else if (which_test == 4)
					{
						cout << "Ilosc potomkow\n";
						file << "Ilosc potomkow\n";
					}
					for (int k = 0; k<how_many_dif; k++)
					{
						cout << "STOP AT: " << arguments[k] << endl;
						file.width(12);
						file << arguments[k];
						time = 0;
						solution = 0;
						for (int j = 1; j <= repeat; j++)
						{
							cout << "REPEAT NUMBER : " << j << endl;
							if (which_test == 0)
							{
								performanceCountStart = startTimer();
								solution += genetic_Algorithm->algorithm(arguments[k], numberOfPopulation, numberOfGenes, probability, arguments[k] / 2);
								performanceCountEnd = endTimer();
							}
							else if (which_test == 1)
							{
								performanceCountStart = startTimer();
								solution += genetic_Algorithm->algorithm(sizeOfPopulation, arguments[k], numberOfGenes, probability, numberOfChild);
								performanceCountEnd = endTimer();
							}
							else if (which_test == 2)
							{
								performanceCountStart = startTimer();
								solution += genetic_Algorithm->algorithm(sizeOfPopulation, numberOfPopulation, arguments[k], probability, numberOfChild);
								performanceCountEnd = endTimer();
							}
							else if (which_test == 3)
							{
								performanceCountStart = startTimer();
								solution += genetic_Algorithm->algorithm(sizeOfPopulation, numberOfPopulation, size*0.1, arguments[k], numberOfChild);
								performanceCountEnd = endTimer();
							}
							else if (which_test == 4)
							{
								performanceCountStart = startTimer();
								solution += genetic_Algorithm->algorithm(sizeOfPopulation, numberOfPopulation, numberOfGenes, probability, size*arguments[k]);
								performanceCountEnd = endTimer();
							}
							time += (performanceCountEnd.QuadPart - performanceCountStart.QuadPart);
						}
						time /= repeat;
						solution /= repeat;
						time = time / freq.QuadPart * 1000;
						file.width(13);
						file << time;
						cout << "Sredni czas = " << time << endl;
						file.width(13);
						file << solution;
						cout << "Sredni wynik = " << solution << endl;
						percent = (solution / (bestSolutions[i] * 1.0) - 1) * 100;
						cout << "Procent bledu = " << percent << endl;
						file.width(13);
						file << percent << endl;
						if (percent == 0) k = how_many_dif;
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

void testTabuAtsp()
{
	int ile_instancji = 7;

	string* files = new string[ile_instancji]();			// nazwy plików z instancjami
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


	int size = tabu_search->getSize();
	//int iterations = 50000;						// liczba iteracji petli glownej algorytmu
	//int not_change = size*4;					// maksymalna liczba iteracji bez poprawy rozwiazania
	int div_not_change = 20;						// mnozone razy size-  max liczba iteracji bez poprawy rozwiazania do zastosowania dywersyfikacji
	//double alg_time = 1;						// czas dzialania algorytmu
	int num_of_candidates = 2;					// mnozone razy size - liczba kandydatów 
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
					delete tabu_search;
					tabu_search = new Tabu_search(files[i]);
					size = tabu_search->getSize();
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
							tabu_search->setParameters(stopCriteria[k], -1, div_not_change*size, -1, num_of_candidates*size, tabu_length, true, 0);
						else if (stop_condition == 1)
							tabu_search->setParameters(-1, -1, div_not_change*size, stopCriteria[k], num_of_candidates*size, tabu_length, true, 1);
						else if (stop_condition == 2)
							tabu_search->setParameters(-1, stopCriteria[k], div_not_change*size, -1, num_of_candidates*size, tabu_length, true, 2);
						for (int j = 1; j <= repeat; j++)
						{
							cout << "REPEAT with diversification : " << j << endl;
							performanceCountStart = startTimer();
							solution += tabu_search->algorithm();
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
							tabu_search->setParameters(stopCriteria[k], -1, div_not_change*size, -1, num_of_candidates*size, tabu_length, false, 0);
						else if (stop_condition == 1)
							tabu_search->setParameters(-1, -1, div_not_change*size, stopCriteria[k], num_of_candidates*size, tabu_length, false, 1);
						else if (stop_condition == 2)
							tabu_search->setParameters(-1, stopCriteria[k], div_not_change*size, -1, num_of_candidates*size, tabu_length, false, 2);
						for (int j = 1; j <= repeat; j++)
						{
							cout << "REPEAT without diversification : " << j << endl;
							performanceCountStart = startTimer();
							solution += tabu_search->algorithm();
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
