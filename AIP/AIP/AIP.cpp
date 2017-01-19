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

//---------- AUXILIARY FUNCTIONS ---------------
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

struct myfillandwr {
    myfillandwr(char f, int w)
        : fill(f), width(w) {}

    char fill;
    int width;
};

struct myfillandwl {
    myfillandwl(char f, int w)
        : fill(f), width(w) {}

    char fill;
    int width;
};

ostream& operator<<(ostream& o, const myfillandwr& a) {
    o.fill(a.fill);
    o.width(a.width);
    return o;
}

ostream& operator<<(ostream& o, const myfillandwl& a) {
    o.fill(a.fill);
    o.width(a.width);
    o.flags(std::ios::left);
    return o;
}

double duration(double &time, int repeat) {
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    time = time / freq.QuadPart;
    time = time / repeat;
    // in [s]
    return time;
}

void printConsole() {
    cout << endl << "console> ";
}

void replaceAll(string& str, const string& from, const string& to) {
    if (from.empty())
        return;
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}

string doubleToString(double value) {
    ostringstream stream;
    stream << value;
    return stream.str();
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

string arrayToString(string array[], int size) {
    string result;
    result = "{";
    int posOfAtsp;
    for (int i = 0; i<size; i++) {
        posOfAtsp = array[i].find(".atsp");
        if (posOfAtsp != string::npos) {
            result += array[i].substr(0, posOfAtsp);
        } else {
            result += array[i];
        }
        if (i != size - 1)
            result += ",";
    }
    result += "}";
    return result;
}

string getCurrentTime() {
    string result;
    time_t t = time(0);
    struct tm now;
    localtime_s(&now, &t);
    char buf[80];
    strftime(buf, sizeof(buf), "%Y-%m-%d_%X", &now);
    result = buf;
    replaceAll(result, ":", "-");
    return result;
}

inline bool fileExists(const std::string& name) {
    ifstream f(name.c_str());
    return f.good();
}

//---------- END ---------------

void geneticAlgorithmTestMenu();
void simulatedAnnealingTestMenu();
void tabuSearchTestMenu();
void geneticAlgorithmMenu(string);
void simulatedAnnealingMenu(string);
void tabuSearchMenu(string);

TabuSearch* tabuSearch;
SimulatedAnnealing* simulatedAnnealing;
GeneticAlgorithm* geneticAlgorithm;

int _tmain(int argc, _TCHAR* argv[]) {

    srand((unsigned int)time(NULL));

    //bool showMenu = true;
    bool isProperlyLoaded = false;
    char choice;
    string fileName = "br17.atsp";
    ostringstream mainMenuStream;
    int sizeOfInstance = 0;

    if (fileExists(fileName)) {
        simulatedAnnealing = new SimulatedAnnealing(fileName);
        tabuSearch = new TabuSearch(fileName);
        geneticAlgorithm = new GeneticAlgorithm(fileName);
        sizeOfInstance = geneticAlgorithm->getSize();
        isProperlyLoaded = sizeOfInstance > 0;
    }

    while (1) {
        if (!isProperlyLoaded) {
            fileName = "No file";
            sizeOfInstance = 0;
        }
        system("cls");
        mainMenuStream.str("");
        mainMenuStream.clear();
        mainMenuStream <<
                       "|.........................................................................|\n"
                       "|.......... ________________________________ .............................|\n"
                       "|..........|      _  _   _                  |.............................|\n"
                       "|..........|     / |/ | |_ |\\ | | |         |.............................|\n"
                       "|..........|    /     | |_ | \\| |_|         |.............................|\n"
                       "|..._______|________________________________|____________________.........|\n"
                       "|..|                                                             |........|\n"
                       "|..|                       ALGORITHMS                            |........|\n"
                       "|..|                                                             |........|\n"
                       "|..|      OPTION                                    KEY          |........|\n"
                       "|..|                                                             |........|\n"
                       "|..|                                                             |........|\n"
                       "|..|      SIMULATED ANNEALING                        S           |........|\n"
                       "|..|                                                             |........|\n"
                       "|..|      TABU SEARCH                                T           |........|\n"
                       "|..|                                                             |........|\n"
                       "|..|      GENETIC ALGORITHM                          G           |........|\n"
                       "|..|                                                             |........|\n"
                       "|..|                                                             |........|\n"
                       "|..|                        OPTIONS                              |........|\n"
                       "|..|                                                             |........|\n"
                       "|..|                                                             |........|\n"
                       "|..|      LOAD FROM FILE (.atsp)                     L           |........|\n"
                       "|..|                                                             |........|\n"
                       "|..|      PREVIEW LOADED DATA                        P           |........|\n"
                       "|..|                                                             |........|\n"
                       "|..|      QUIT                                       Q           |........|\n"
                       "|..|                                                             |........|\n"
                       "|..|________________   INSTANCE   _______________________________|........|\n"
                       "|..|                                                             |........|\n"
                       "|..|                                                             |........|\n"
                       "|..|      FILENAME :                        " << myfillandwr(' ', 10) << fileName << "           |........|\n"
                       "|..|                                                             |........|\n"
                       "|..|      NUMBER OF CITIES :                       " << myfillandwr(' ', 3) << sizeOfInstance << "           |........|\n"
                       "|..|                                                             |........|\n"
                       "|..|________________   AUTHORS    _______________________________|........|\n"
                       "|..|                                                             |........|\n"
                       "|..|                                                             |........|\n"
                       "|..|      MICHAL SZTUKA                        200798            |........|\n"
                       "|..|      JOANNA TUROWSKA                      236088            |........|\n"
                       "|..|                                                             |........|\n"
                       "|..|_____________________________________________________________|........|\n"
                       "|.........................................................................|\n"
                       "|.........................................................................|\n";
        cout << mainMenuStream.str() << endl;


        if (isProperlyLoaded) {
            cout << endl << "Loaded from file: " << fileName <<endl;
        } else {
            cout << endl << "There was a problem with loading data from file."<<endl;
        }

        //showMenu = false;
        printConsole();
        cin >> choice;

        if (!isProperlyLoaded && choice != 'L' && choice != 'l' && choice != 'q' && choice != 'Q') {
            cout << endl << "Try to load data from another file.";
            //just to ommit switch clause
            choice = 'o';
        } else if (choice != 'l' && choice != 'L') {
            //showMenu = true;
        }

        switch (choice) {
        case 's':
        case 'S':
            simulatedAnnealingMenu(fileName);
            break;
        case 't':
        case 'T':
            tabuSearchMenu(fileName);
            break;
        case 'g':
        case 'G':
            geneticAlgorithmMenu(fileName);
            break;
        case 'l':
        case 'L':
            fileName = "";
            cout << endl << "Input filename : ";
            cin >> fileName;
            if (fileName.find(".atsp") == string::npos) {
                fileName += ".atsp";
            }
            if (fileExists(fileName)) {
                delete simulatedAnnealing;
                delete tabuSearch;
                delete geneticAlgorithm;

                simulatedAnnealing = new SimulatedAnnealing(fileName);
                tabuSearch = new TabuSearch(fileName);
                geneticAlgorithm = new GeneticAlgorithm(fileName);

                sizeOfInstance = geneticAlgorithm->getSize();
                isProperlyLoaded = sizeOfInstance > 0;
            } else {
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
            delete simulatedAnnealing;
            delete tabuSearch;
            delete geneticAlgorithm;
            exit(0);
        default:
            break;
        }
    }

    system("PAUSE");
    return 0;
}

void geneticAlgorithmMenu(string filename) {
    ostringstream menuStream;
    char choice;
    bool goBack = false;
    bool showMenu = true;
    double time = 0;
    LARGE_INTEGER performanceCountStart, performanceCountEnd;
    int solution = 0;
    int newValue;
    bool setAll = false;

    while (!goBack) {
        setAll = false;
        menuStream.str("");
        menuStream.clear();
        menuStream <<
                   "|.........................................................................|\n"
                   "|.......... ________________________________ .............................|\n"
                   "|..........|      _  _   _                  |.............................|\n"
                   "|..........|     / |/ | |_ |\\ | | |         |.............................|\n"
                   "|..........|    /     | |_ | \\| |_|         |.............................|\n"
                   "|..._______|________________________________|____________________.........|\n"
                   "|..|                                                             |........|\n"
                   "|..|                   GENETIC ALGORITHM                         |........|\n"
                   "|..|                                                             |........|\n"
                   "|..|      OPTION                                    KEY          |........|\n"
                   "|..|                                                             |........|\n"
                   "|..|                                                             |........|\n"
                   "|..|      START SINGLE SOLUTION                      S           |........|\n"
                   "|..|                                                             |........|\n"
                   "|..|      MAKE TESTS                                 T           |........|\n"
                   "|..|                                                             |........|\n"
                   "|..|      CHANGE VALUE OF PARAMETER NR. X            X           |........|\n"
                   "|..|                                                             |........|\n"
                   "|..|      SET ALL                                    A           |........|\n"
                   "|..|                                                             |........|\n"
                   "|..|                                                             |........|\n"
                   "|..|      BACK                                       B           |........|\n"
                   "|..|                                                             |........|\n"
                   "|..|________________   INSTANCE   _______________________________|........|\n"
                   "|..|                                                             |........|\n"
                   "|..|                                                             |........|\n"
                   "|..|      FILENAME :                        " << myfillandwr(' ', 10) << filename << "           |........|\n"
                   "|..|                                                             |........|\n"
                   "|..|      NUMBER OF CITIES :                       " << myfillandwr(' ', 3) << geneticAlgorithm->getSize() << "           |........|\n"
                   "|..|                                                             |........|\n"
                   "|..|_____________   PARAMETERS OF ALGORITHM   ___________________|........|\n"
                   "|..|                                                             |........|\n"
                   "|..|                                                             |........|\n"
                   "|..|   1. SIZE OF POPULATION                       " << myfillandwr(' ', 3) << geneticAlgorithm->sizeOfPopulation << "           |........|\n"
                   "|..|                                                             |........|\n"
                   "|..|   2. POPULATION QUANTITY                      " << myfillandwr(' ', 3) << geneticAlgorithm->numberOfPopulations << "           |........|\n"
                   "|..|                                                             |........|\n"
                   "|..|   3. NUMBER OF GENES                          " << myfillandwr(' ', 3) << geneticAlgorithm->numberOfGenes << "           |........|\n"
                   "|..|                                                             |........|\n"
                   "|..|   4. PROBABILITY OF MUTATION                  " << myfillandwr(' ', 3) << geneticAlgorithm->probability << "           |........|\n"
                   "|..|                                                             |........|\n"
                   "|..|   5. CHILDREN QUANTITY                        " << myfillandwr(' ', 3) << geneticAlgorithm->numberOfChildren << "           |........|\n"
                   "|..|                                                             |........|\n"
                   "|..|_____________________________________________________________|........|\n"
                   "|.........................................................................|\n"
                   "|.........................................................................|\n"
                   "|.........................................................................|\n"
                   "|.........................................................................|\n";
        if (showMenu) {
            system("cls");
            cout << menuStream.str();
        }
        printConsole();
        cin >> choice;
        switch (choice) {
        case 'a':
        case 'A':
            setAll = true;
        case '1':
            cout << "Input new value for size of population (>0): ";
            cin >> newValue;
            if (newValue>0) geneticAlgorithm->sizeOfPopulation = newValue;
            if (!setAll)
                break;
        case '2':
            cout << "Input new value for quantity of populations (>0): ";
            cin >> newValue;
            if (newValue>0) geneticAlgorithm->numberOfPopulations = newValue;
            if (!setAll)
                break;
        case '3':
            cout << "Input new value for probability of mutation (0 < x <100): ";
            cin >> newValue;
            if (newValue>0) geneticAlgorithm->probability = newValue;
            if (!setAll)
                break;
        case '4':
            cout << "Input new value for number of genes (>0): ";
            cin >> newValue;
            if (newValue>0) geneticAlgorithm->numberOfGenes = newValue;
            if (!setAll)
                break;
        case '5':
            cout << "Input new value for number of children (>0): ";
            cin >> newValue;
            if (newValue>0) geneticAlgorithm->numberOfChildren = newValue;
            break;
        case 's':
        case 'S':
            time = 0;
            cout << "\nPlease wait... Computing...\n\n";
            performanceCountStart = startTimer();
            solution = geneticAlgorithm->algorithm();
            performanceCountEnd = endTimer();
            time = (double)(performanceCountEnd.QuadPart - performanceCountStart.QuadPart);
            duration(time, 1);
            cout << "Solution: " << solution;
            cout << "\nComputation time: " << time << " [ms]\n\n";
            cout << "Press any key to continue...";
            cin.ignore();
            cin.get();
            break;
        case 't':
        case 'T':
            geneticAlgorithmTestMenu();
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

void tabuSearchMenu(string filename) {
    int size = tabuSearch->getSize();

    int iterations = 50000;					// liczba iteracji petli glownej algorytmu
    int not_change = size * 4;					// maksymalna liczba iteracji bez poprawy rozwiazania
    int div_not_change = size * 2;
    double alg_time = 1;				// czas dzialania algorytmu
    int num_of_candidates = size;			// liczba kandydatów
    int tabu_length = 10;				// dlugosc listy tabu
    bool diversificationOn = true;			// 0 - wylaczona 1 - wlaczona
    int stop_condition = 0;				// 0 - interacje, 1 - time, 2 - not_change
    int solution = 0;

    ostringstream tabuMenuStream;
    string tabuMenu;
    LARGE_INTEGER performanceCountStart, performanceCountEnd;
    char option;
    bool goBack = false;
    bool showMenu = true;
    bool loop = true;
    bool loop2 = true;
    bool loop3 = true;
    int ktory;

    while (!goBack) {
        bool setAll = false;
        tabuMenuStream.str("");
        tabuMenuStream.clear();
        tabuMenuStream <<
                       "|.........................................................................|\n"
                       "|.......... ________________________________ .............................|\n"
                       "|..........|      _  _   _                  |.............................|\n"
                       "|..........|     / |/ | |_ |\\ | | |         |.............................|\n"
                       "|..........|    /     | |_ | \\| |_|         |.............................|\n"
                       "|..._______|________________________________|____________________.........|\n"
                       "|..|                                                             |........|\n"
                       "|..|                   TABU SEARCH                               |........|\n"
                       "|..|                                                             |........|\n"
                       "|..|   OPTION:                                      KEY:         |........|\n"
                       "|..|                                                             |........|\n"
                       "|..|                                                             |........|\n"
                       "|..|   START SINGLE SOLUTION                         S           |........|\n"
                       "|..|                                                             |........|\n"
                       "|..|   MAKE TESTS                                    T           |........|\n"
                       "|..|                                                             |........|\n"
                       "|..|   CHANGE VALUE OF PARAMETER NR. X               X           |........|\n"
                       "|..|                                                             |........|\n"
                       "|..|   SET ALL                                       A           |........|\n"
                       "|..|                                                             |........|\n"
                       "|..|                                                             |........|\n"
                       "|..|   GO BACK                                       B           |........|\n"
                       "|..|                                                             |........|\n"
                       "|..|________________   INSTANCE   _______________________________|........|\n"
                       "|..|                                                             |........|\n"
                       "|..|                                                             |........|\n"
                       "|..|   FILENAME :                           " << myfillandwr(' ', 10) << filename << "           |........|\n"
                       "|..|                                                             |........|\n"
                       "|..|   NUMBER OF CITIES :                          " << myfillandwr(' ', 3) << size << "           |........|\n"
                       "|..|                                                             |........|\n"
                       "|..|                                                             |........|\n"
                       "|..|_____________   PARAMETERS OF ALGORITHM   ___________________|........|\n"
                       "|..|                                                             |........|\n"
                       "|..|                                                             |........|\n"
                       "|..|  1. STOPPING CONDITION                        " << myfillandwr(' ', 3) << stop_condition << "           |........|\n"
                       "|..|                                                             |........|\n"
                       "|..|  2. DIVERSIFICATION (1-yes,0-no)              " << myfillandwr(' ', 3) << diversificationOn << "           |........|\n"
                       "|..|                                                             |........|\n"
                       "|..|  3. PARAMETER not_change FOR DIVERSIFICATION  " << myfillandwr(' ', 3) << div_not_change << "           |........|\n"
                       "|..|                                                             |........|\n"
                       "|..|  4. LENGHT OF TABU LIST                       " << myfillandwr(' ', 3) << tabu_length << "           |........|\n"
                       "|..|                                                             |........|\n"
                       "|..|  5. NUMBER OF CANDIDATES                      " << myfillandwr(' ', 3) << num_of_candidates << "           |........|\n"
                       "|..|                                                             |........|\n"
                       "|..|_____________________________________________________________|........|\n"
                       "|.........................................................................|\n"
                       "|.........................................................................|\n"
                       "|.........................................................................|\n"
                       "|.........................................................................|\n";

        tabuMenu = tabuMenuStream.str();
        double time = 0;
        if (showMenu) {
            system("cls");
            cout << tabuMenu;
        }
        printConsole();
        cin >> option;
        switch (option) {
        case 'a':
        case 'A':
            setAll = true;
        case '1':
            cout << "\nChoose stopping condition: \n0 - iterations, 1 - time, 2 - no change of solution\n";
            cin >> ktory;
            if (ktory >= 0 && ktory <3) stop_condition = ktory;
            if (!setAll)
                break;
        case '2':
            cout << "\nDiversification?\n1 - yes, 0 - no\n";
            cin >> ktory;
            if (ktory == 1)  diversificationOn = true;
            else if (ktory == 0) diversificationOn = false;
            if (!setAll)
                break;
        case '3':
            cout << "\nInsert value of the parameter not_change for diversification (e.g.not_change=2*size) : ";
            cin >> ktory;
            if (ktory>0) div_not_change = ktory;
            if (!setAll)
                break;
        case '4':
            do {
                cout << "\nInsert the lengh of tabu list : ";
                cin >> ktory;
            } while (ktory <= 0);
            tabu_length = ktory;
            if (!setAll)
                break;
        case '5':
            cout << "\nInsert the number of candidates (e.g.num_of_candidates=2*size) : ";
            cin >> ktory;
            if (ktory>1) num_of_candidates = ktory;
            break;
        case 's':
        case 'S':
            cout << "\nPlease wait... Computing...\n\n";
            tabuSearch->setParameters(iterations, not_change, div_not_change, alg_time, num_of_candidates, tabu_length, diversificationOn, stop_condition);
            performanceCountStart = startTimer();
            solution = tabuSearch->algorithm();
            performanceCountEnd = endTimer();
            time = (double)(performanceCountEnd.QuadPart - performanceCountStart.QuadPart);
            duration(time, 1);
            cout << "Solution: " << solution;
            cout << "\nComputation time: " << time << " [ms]\n\n";
            cout << "Press any key to continue...";
            cin.ignore();
            cin.get();
            break;
        case 't':
        case 'T':
            tabuSearchTestMenu();
            break;
        case 'b':
        case 'B':
            goBack = true;
            break;
        default:
            break;
        }
        showMenu = option != 'S' && option != 's';
    }
};

void simulatedAnnealingMenu(string filename) {
    double alphas[5] = { 0.9, 0.95, 0.99, 0.995, 0.999 };
    double ile = 10.0;

    int solution = 0;
    double time = 0;

    ostringstream simulatedMenuStream;
    LARGE_INTEGER performanceCountStart, performanceCountEnd;

    char choice;
    char choice2;
    bool goBack = false;
    bool showMenu = true;
    bool setAll = false;
    int newValue;
    double newDoubleValue;
    string initialTemperatureStr;
    string variantStr;

    while (!goBack) {
        setAll = false;
        simulatedMenuStream.str("");
        simulatedMenuStream.clear();
        initialTemperatureStr = simulatedAnnealing->autoGenerateInitialTemperature ? "Auto" : doubleToString(simulatedAnnealing->initialTemperature);
        variantStr = simulatedAnnealing->blackAndWhite ? "Black and White" : "Classic";
        simulatedMenuStream <<
                            "|.........................................................................|\n"
                            "|.......... ________________________________ .............................|\n"
                            "|..........|      _  _   _                  |.............................|\n"
                            "|..........|     / |/ | |_ |\\ | | |         |.............................|\n"
                            "|..........|    /     | |_ | \\| |_|         |.............................|\n"
                            "|..._______|________________________________|____________________.........|\n"
                            "|..|                                                             |........|\n"
                            "|..|                                                             |........|\n"
                            "|..|                   SIMULATED ANNEALING                       |........|\n"
                            "|..|                                                             |........|\n"
                            "|..|      OPTION                                    KEY          |........|\n"
                            "|..|                                                             |........|\n"
                            "|..|                                                             |........|\n"
                            "|..|      START SINGLE SOLUTION                      S           |........|\n"
                            "|..|                                                             |........|\n"
                            "|..|      MAKE TESTS                                 T           |........|\n"
                            "|..|                                                             |........|\n"
                            "|..|      CHANGE VALUE OF PARAMETER NR. X            X           |........|\n"
                            "|..|                                                             |........|\n"
                            "|..|      SET ALL                                    A           |........|\n"
                            "|..|                                                             |........|\n"
                            "|..|                                                             |........|\n"
                            "|..|      GO BACK                                    B           |........|\n"
                            "|..|                                                             |........|\n"
                            "|..|________________   INSTANCE   _______________________________|........|\n"
                            "|..|                                                             |........|\n"
                            "|..|                                                             |........|\n"
                            "|..|      FILENAME :                        " << myfillandwr(' ', 10) << filename << "           |........|\n"
                            "|..|                                                             |........|\n"
                            "|..|      NUMBER OF CITIES :                       " << myfillandwr(' ', 3) << simulatedAnnealing->getSize() << "           |........|\n"
                            "|..|                                                             |........|\n"
                            "|..|                                                             |........|\n"
                            "|..|_____________   PARAMETERS OF ALGORITHM   ___________________|........|\n"
                            "|..|                                                             |........|\n"
                            "|..|                                                             |........|\n"
                            "|..|   1. DURATION (ERA)                          " << myfillandwr(' ', 4) << simulatedAnnealing->period << "           |........|\n"
                            "|..|                                                             |........|\n"
                            "|..|   2. PARAMETER ALPHA                        " << myfillandwr(' ', 5) << simulatedAnnealing->alpha << "           |........|\n"
                            "|..|                                                             |........|\n"
                            "|..|   3. FINAL TEMPERATURE                     " << myfillandwr(' ', 6) << simulatedAnnealing->finalTemperature << "           |........|\n"
                            "|..|                                                             |........|\n"
                            "|..|   4. INITIAL TEMPERATURE                   " << myfillandwr(' ', 6) << initialTemperatureStr << "           |........|\n"
                            "|..|                                                             |........|\n"
                            "|..|   5. TSP VARIANT       " << myfillandwr(' ', 26) << variantStr << "           |........|\n"
                            "|..|                                                             |........|\n"
                            "|..|_____________________________________________________________|........|\n"
                            "|.........................................................................|\n"
                            "|.........................................................................|\n"
                            "|.........................................................................|\n"
                            "|.........................................................................|\n";
        if (showMenu) {
            system("cls");
            cout << simulatedMenuStream.str();
        }
        printConsole();
        cin >> choice;

        switch (choice) {
        case 'a':
        case 'A':
            setAll = true;
        case '1':
            cout << "Input new value for duration (era) ( > 0 ): ";
            cin >> newValue;
            if (newValue>0) simulatedAnnealing->period = newValue;
            if (!setAll)
                break;
        case '2':
            cout << "Input new value for alpha ( 0 < alpha < 1 ): ";
            cin >> newDoubleValue;
            if (newDoubleValue>0 && newDoubleValue<1) simulatedAnnealing->alpha = newDoubleValue;
            if (!setAll)
                break;
        case '3':
            cout << "Input new value for final temperature ( > 0 ): ";
            cin >> newDoubleValue;
            if (newDoubleValue>0) simulatedAnnealing->finalTemperature = newDoubleValue;
            if (!setAll)
                break;
        case '4':
            cout << "Do you want to auto-generate initial temperature ? (y/n) : ";
            cin >> choice2;
            if (choice2 == 'y') {
                simulatedAnnealing->autoGenerateInitialTemperature = true;
            } else {
                simulatedAnnealing->autoGenerateInitialTemperature = false;
                cout << "Input new value for initial temperature ( > " << simulatedAnnealing->finalTemperature << " ): ";
                cin >> newDoubleValue;
                if (newDoubleValue>simulatedAnnealing->finalTemperature) simulatedAnnealing->initialTemperature = newDoubleValue;
            }
            if (!setAll)
                break;
        case '5':
            cout << "Which variant of TSP problem do you want to solve ?";
            cout << "\n\t 1 - Classic";
            cout << "\n\t 2 - Black and White";
            cout << "\nYour choice: ";
            cin >> newValue;
            if (newValue == 1)
                simulatedAnnealing->blackAndWhite = false;
            else
                simulatedAnnealing->blackAndWhite = true;
            break;
        case 's':
        case 'S':
            time = 0;
            cout << "\nPlease wait... Computing..." << endl;
            performanceCountStart = startTimer();
            solution = simulatedAnnealing->algorithm();
            performanceCountEnd = endTimer();
            time = (double)(performanceCountEnd.QuadPart - performanceCountStart.QuadPart);
            duration(time, 1);
            if (simulatedAnnealing->autoGenerateInitialTemperature) {
                cout << "\n\t Initial temperature: " << simulatedAnnealing->initialTemperature << endl;
            }
            cout << "\n\t Solution: " << solution << endl;
            cout << "\n\t Computation time: " << time << " [ms]" << endl;
            cout << "\n\t " << simulatedAnnealing->bestPathToString() << endl;
            break;
        case 't':
        case 'T':
            simulatedAnnealingTestMenu();
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

void simulatedAnnealingTestMenu() {

    LARGE_INTEGER performanceCountStart, performanceCountEnd;
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    ofstream resultFile;

    ostringstream testMenuStream;
    string dataFilesStr;
    string bestKnownSolutionsStr;
    string valuesOfTestedFactorStr;
    string resultFileStr = getCurrentTime() + ".csv";

    int numOfInstances = 7;
    string* dataFiles = new string[numOfInstances]();
    dataFiles[0] = "br17.atsp";
    dataFiles[1] = "ftv47.atsp";
    dataFiles[2] = "ftv64.atsp";
    dataFiles[3] = "kro124p.atsp";
    dataFiles[4] = "ftv170.atsp";
    dataFiles[5] = "rbg323.atsp";
    dataFiles[6] = "rbg443.atsp";

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

    int testScenario = 3;
    string possibleTestScenario[4] = { "Initial temperature", "Final temperature", "Parameter alpha", "Period" };
    int repetitionsOfTestCase = 2;
    int solution = 0;

    char choice;
    bool goBack = false;
    bool setAll = false;
    bool stopTest = false;
    bool showMenu = true;

    double time = 0;
    double percentangeError = 0;

    int newValue;
    string newStringValue;

    while (!goBack) {
        stopTest = false;
        setAll = false;
        dataFilesStr = arrayToString(dataFiles, numOfInstances);
        bestKnownSolutionsStr = arrayToString(bestKnownSolutions, numOfInstances);
        valuesOfTestedFactorStr = arrayToString(valuesOfTestedFactor, numOfTestedValues);

        testMenuStream.str("");
        testMenuStream.clear();
        testMenuStream <<
                       "|.........................................................................|\n"
                       "|.......... ________________________________ .............................|\n"
                       "|..........|      _  _   _                  |.............................|\n"
                       "|..........|     / |/ | |_ |\\ | | |         |.............................|\n"
                       "|..........|    /     | |_ | \\| |_|         |.............................|\n"
                       "|..._______|________________________________|____________________.........|\n"
                       "|..|                                                             |........|\n"
                       "|..|                   SIMULATED ANNEALING                       |........|\n"
                       "|..|                                                             |........|\n"
                       "|..|      OPTION:                                   KEY:         |........|\n"
                       "|..|                                                             |........|\n"
                       "|..|                                                             |........|\n"
                       "|..|      START TEST                                 S           |........|\n"
                       "|..|                                                             |........|\n"
                       "|..|      CHANGE VALUE OF PARAMETER NR. X            X           |........|\n"
                       "|..|                                                             |........|\n"
                       "|..|      SET ALL                                    A           |........|\n"
                       "|..|                                                             |........|\n"
                       "|..|      BACK                                       B           |........|\n"
                       "|..|                                                             |........|\n"
                       "|..|_____________   PARAMETERS OF TEST   ________________________|........|\n"
                       "|..|                                                             |........|\n"
                       "|..|                                                             |........|\n"
                       "|..|   1. NUMBER OF INSTANCES                      " << myfillandwr(' ', 3) << numOfInstances << "           |........|\n"
                       "|..|                                                             |........|\n"
                       "|..|      " << myfillandwl(' ', 49) << dataFilesStr << "      |........|\n"
                       "|..|                                                             |........|\n"
                       "|..|   2. BEST KNOWN SOLUTIONS                                   |........|\n"
                       "|..|                                                             |........|\n"
                       "|..|      " << myfillandwl(' ', 49) << bestKnownSolutionsStr << "      |........|\n"
                       "|..|                                                             |........|\n"
                       "|..|   3. TESTED FACTOR                                          |........|\n"
                       "|..|                                                             |........|\n"
                       "|..|      " << myfillandwl(' ', 26) << possibleTestScenario[testScenario] << "                             |........|\n"
                       "|..|                                                             |........|\n"
                       "|..|   4. VALUES OF TESTED FACTOR                                |........|\n"
                       "|..|                                                             |........|\n"
                       "|..|      " << myfillandwl(' ', 26) << valuesOfTestedFactorStr << "                             |........|\n"
                       "|..|                                                             |........|\n"
                       "|..|   5. REPETITIONS OF TEST CASE                   " << myfillandwl(' ', 3) << repetitionsOfTestCase << "         |........|\n"
                       "|..|                                                             |........|\n"
                       "|..|   6. NAME OF RESULT FILE                                    |........|\n"
                       "|..|                                                             |........|\n"
                       "|..|      " << myfillandwl(' ', 26) << resultFileStr << "                             |........|\n"
                       "|..|_____________________________________________________________|........|\n"
                       "|.........................................................................|\n"
                       "|.........................................................................|\n"
                       "|.........................................................................|\n"
                       "|.........................................................................|\n";
        if (showMenu) {
            system("cls");
            cout << testMenuStream.str();
        }
        showMenu = true;
        printConsole();
        cin >> choice;

        if (choice == 's' || choice == 'S') {
            showMenu = false;
        }

        switch (choice) {
        case 'a':
        case 'A':
            setAll = true;
        case '1':
            cout << "Input new value for instances quantity (>0): ";
            cin >> newValue;
            if (newValue <= 0)
                break;
            numOfInstances = newValue;
            cout << "Provide names of data files (.atsp) : ";
            for (int i = 1; i <= numOfInstances; i++) {
                cout << endl << i << " of " << numOfInstances << " : ";
                cin >> newStringValue;
                if (newStringValue.find(".atsp") == string::npos) {
                    newStringValue += ".atsp";
                }

                if (!fileExists(newStringValue)) {
                    cout << endl << "There is no such file in directory, try again.";
                    i--;
                } else {
                    dataFiles[i - 1] = newStringValue;
                }
            }
        case '2':
            cout << "Provide best known solutions for these instances." << endl;
            for (int i = 1; i <= numOfInstances; i++) {
                cout << endl << i << " of " << numOfInstances << " : ";
                cin >> bestKnownSolutions[i - 1];
            }
            if (!setAll)
                break;
        case '3':
            cout << "Which factor you want to test?";
            for (int i = 0; i < (sizeof(possibleTestScenario) / sizeof(*possibleTestScenario)); i++) {
                cout << "\n\t" << i << " - " + possibleTestScenario[i];
            }
            cout << "\nYour choice: ";
            cin >> newValue;
            if (newValue >= 0 && newValue <= 4) testScenario = newValue;
            if (!setAll)
                break;
        case '4':
            cout << "How many values do you want to test? ";
            cin >> numOfTestedValues;
            cout << "Provide values of tested factor \n ";
            for (int i = 1; i <= numOfTestedValues; i++) {
                cout << endl << i << " of " << numOfTestedValues << " : ";
                cin >> valuesOfTestedFactor[i - 1];
            }
            if (!setAll)
                break;
        case '5':
            cout << "How many repetitions of test case ? (results will be averaged) (>0): ";
            cin >> newValue;
            if (newValue>0)
                repetitionsOfTestCase = newValue;
            if (!setAll)
                break;
        case '6':
            do {
                cout << endl << "Input name of result file (.csv): ";
                cin >> newStringValue;
            } while (resultFileStr.empty());

            if (newStringValue.find(".csv") != string::npos) {
            } else {
                newStringValue += ".csv";
            }
            if (fileExists(newStringValue)) {
                cout << "There is a file with this name. Are you sure ? (y/n) \n : ";
                cin >> choice;
                if (choice == 'y')
                    resultFileStr = newStringValue;
            } else {
                resultFileStr = newStringValue;
            }
            break;
        case 's':
        case 'S':
            for (int i = 0; i < numOfInstances; i++) {
                if (!fileExists(dataFiles[i])) {
                    cout << "File " << dataFiles[i] << " does not exist. Can not start test..";
                    stopTest = true;
                    break;
                }
            }
            if (stopTest)
                break;
            resultFile.open(resultFileStr.c_str());
            if (!resultFile.good()) {
                cout << "\nError with creating file.";
                cout << endl << "Press any key to continue...";
                cin.ignore();
                cin.get();
            } else {

                cout << "Test started. Please wait, it may take a while..." << endl;
                cout << "TESTED FACTOR: " << possibleTestScenario[testScenario] << endl;
                cout << "Number of runs per single scenario : " << repetitionsOfTestCase << endl;

                resultFile << "Results of testing simulated annealing\n;parameter;alpha;final temperature;period;initial temperature; tsp version \n";
                resultFile << ";value;" << simulatedAnnealing->alpha << ";" << simulatedAnnealing->finalTemperature << ";" << simulatedAnnealing->period << ";";
                if (!simulatedAnnealing->autoGenerateInitialTemperature)
                    resultFile << simulatedAnnealing->initialTemperature << ";";
                else
                    resultFile << "Auto;";
                if (simulatedAnnealing->blackAndWhite)
                    resultFile << "Black and white";
                else
                    resultFile << "Classic";
                resultFile << "\nTESTED FACTOR: " << possibleTestScenario[testScenario] << "\n";
                resultFile << "Number of runs per single scenario : " << repetitionsOfTestCase << " It means that every measurement is average of " << repetitionsOfTestCase << " runs" << "\n";

                for (int i = 0; i<numOfInstances; i++) {

                    cout << "Test for instance: " << dataFiles[i] << "\nLoading...\n";
                    resultFile << ";" << dataFiles[i] << "\n";
                    resultFile << ";;Value of factor; time[ms]; solution; percentage error\n";

                    delete simulatedAnnealing;
                    simulatedAnnealing = new SimulatedAnnealing(dataFiles[i]);

                    for (int k = 0; k<numOfTestedValues; k++) {
                        cout << endl << "\t" << k + 1 << " value of " << numOfTestedValues << " : " << valuesOfTestedFactor[k] << endl;
                        resultFile << ";;" << valuesOfTestedFactor[k];
                        time = 0;
                        solution = 0;

                        switch (testScenario) {
                        case 0:
                            simulatedAnnealing->initialTemperature = valuesOfTestedFactor[k];
                            break;
                        case 1:
                            simulatedAnnealing->finalTemperature = valuesOfTestedFactor[k];
                            break;
                        case 2:
                            simulatedAnnealing->alpha = valuesOfTestedFactor[k];
                            break;
                        case 3:
                            simulatedAnnealing->period = (int)valuesOfTestedFactor[k];
                            break;
                        }

                        for (int j = 1; j <= repetitionsOfTestCase; j++) {
                            cout << "\t\tIteration " << j << " of " << repetitionsOfTestCase << "..." << endl;
                            performanceCountStart = startTimer();
                            solution += simulatedAnnealing->algorithm();
                            performanceCountEnd = endTimer();
                            time += (performanceCountEnd.QuadPart - performanceCountStart.QuadPart);
                        }

                        time /= repetitionsOfTestCase;
                        solution /= repetitionsOfTestCase;
                        time = time / freq.QuadPart * 1000;

                        resultFile << ";" << time;
                        cout << "\tMean time [ms] : " << time << endl;

                        resultFile << ";" << solution;
                        cout << "\tMean solution : " << solution << endl;

                        percentangeError = (solution / (bestKnownSolutions[i] * 1.0) - 1) * 100;
                        cout << "\tMean percentage error = " << percentangeError << endl;
                        resultFile << ";" << percentangeError << "\n";
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
};

void geneticAlgorithmTestMenu() {
    LARGE_INTEGER performanceCountStart, performanceCountEnd;
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    ofstream resultFile;

    ostringstream geneticTestMenuStream;
    string dataFilesStr;
    string bestKnownSolutionsStr;
    string valuesOfTestedFactorStr;
    string resultFileStr = getCurrentTime() + ".csv" ;

    int numOfInstances = 7;
    // nazwy plików z instancjami
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
    int repetitionsOfTestCase = 2;
    // zmienna na rozwiazanie
    int solution = 0;

    char choice;
    bool goBack = false;
    bool setAll = false;
    bool showMenu = true;
    double time = 0;
    double percentangeError = 0;
    int newValue;
    string newStringValue;
    bool stopTest = false;

    string possibleTestScenario[5] = { "Size of population", "Population quantity", "Quantity of mutating genes", "Probability of mutation", "Children quantity" };

    while (!goBack) {
        stopTest = false;
        setAll = false;
        dataFilesStr = arrayToString(dataFiles, numOfInstances);
        bestKnownSolutionsStr = arrayToString(bestKnownSolutions, numOfInstances);
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
                              "|..|                                                             |........|\n"
                              "|..|      OPTION:                                   KEY:         |........|\n"
                              "|..|                                                             |........|\n"
                              "|..|                                                             |........|\n"
                              "|..|      START TEST                                 S           |........|\n"
                              "|..|                                                             |........|\n"
                              "|..|      CHANGE VALUE OF PARAMETER NR. X            X           |........|\n"
                              "|..|                                                             |........|\n"
                              "|..|      SET ALL                                    A           |........|\n"
                              "|..|                                                             |........|\n"
                              "|..|      BACK                                       B           |........|\n"
                              "|..|                                                             |........|\n"
                              "|..|_____________   PARAMETERS OF TEST   ________________________|........|\n"
                              "|..|                                                             |........|\n"
                              "|..|                                                             |........|\n"
                              "|..|   1. NUMBER OF INSTANCES                      " << myfillandwr(' ', 3) << numOfInstances << "           |........|\n"
                              "|..|                                                             |........|\n"
                              "|..|      "<< myfillandwl(' ', 49)<< dataFilesStr <<"      |........|\n"
                              "|..|                                                             |........|\n"
                              "|..|   2. BEST KNOWN SOLUTIONS                                   |........|\n"
                              "|..|                                                             |........|\n"
                              "|..|      " << myfillandwl(' ', 49) << bestKnownSolutionsStr <<"      |........|\n"
                              "|..|                                                             |........|\n"
                              "|..|   3. TESTED FACTOR                                          |........|\n"
                              "|..|                                                             |........|\n"
                              "|..|      " << myfillandwl(' ', 26) << possibleTestScenario[testScenario] << "                             |........|\n"
                              "|..|                                                             |........|\n"
                              "|..|   4. VALUES OF TESTED FACTOR                                |........|\n"
                              "|..|                                                             |........|\n"
                              "|..|      " << myfillandwl(' ', 26) << valuesOfTestedFactorStr << "                             |........|\n"
                              "|..|                                                             |........|\n"
                              "|..|   5. REPETITIONS OF TEST CASE                   " << myfillandwl(' ', 3) << repetitionsOfTestCase << "         |........|\n"
                              "|..|                                                             |........|\n"
                              "|..|   6. NAME OF RESULT FILE                                    |........|\n"
                              "|..|                                                             |........|\n"
                              "|..|      " << myfillandwl(' ', 26) << resultFileStr << "                             |........|\n"
                              "|..|_____________________________________________________________|........|\n"
                              "|.........................................................................|\n"
                              "|.........................................................................|\n"
                              "|.........................................................................|\n"
                              "|.........................................................................|\n";
        if (showMenu) {
            system("cls");
            cout << geneticTestMenuStream.str();
        }
        showMenu = true;
        printConsole();
        cin >> choice;

        if (choice == 's' || choice == 'S') {
            showMenu = false;
        }

        switch (choice) {
        case 'a':
        case 'A':
            setAll = true;
        case '1':
            cout << "Input new value for instances quantity (>0): ";
            cin >> newValue;
            if (newValue <= 0)
                break;
            numOfInstances = newValue;
            cout << "Provide names of data files (.atsp) : ";
            for (int i = 1; i<=numOfInstances; i++) {
                cout << endl << i << " of " << numOfInstances << " : ";
                cin >> newStringValue;
                if (newStringValue.find(".atsp") == string::npos) {
                    newStringValue += ".atsp";
                }

                if (!fileExists(newStringValue)) {
                    cout << endl << "There is no such file in directory, try again.";
                    i--;
                } else {
                    dataFiles[i - 1] = newStringValue;
                }
            }
        case '2':
            cout << "Provide best known solutions for these instances." << endl;
            for (int i = 1; i<=numOfInstances; i++) {
                cout << endl << i << " of " << numOfInstances << " : ";
                cin >> bestKnownSolutions[i-1];
            }
            if(!setAll)
                break;
        case '3':
            cout << "Which factor you want to test?";
            for (int i = 0; i < (sizeof(possibleTestScenario) / sizeof(*possibleTestScenario)); i++) {
                cout << "\n\t" << i << " - " + possibleTestScenario[i];
            }
            cout << "\nYour choice: ";
            cin >> newValue;
            if (newValue >= 0 && newValue <= 4) testScenario = newValue;
            if (!setAll)
                break;
        case '4':
            cout << "How many values do you want to test? ";
            cin >> numOfTestedValues;
            cout << "Provide values of tested factor \n ";
            for (int i = 1; i<=numOfTestedValues; i++) {
                cout << endl << i << " of " << numOfTestedValues << " : ";
                cin >> valuesOfTestedFactor[i-1];
            }
            if (!setAll)
                break;
        case '5':
            cout << "How many repetitions of test case ? (results will be averaged) (>0): ";
            cin >> newValue;
            if (newValue>0)
                repetitionsOfTestCase = newValue;
            if (!setAll)
                break;
        case '6':
            do {
                cout << endl << "Input name of result file (.csv): ";
                cin >> newStringValue;
            } while (resultFileStr.empty());

            if (newStringValue.find(".csv") != string::npos) {
            } else {
                newStringValue += ".csv";
            }
            if (fileExists(newStringValue)) {
                cout << "There is a file with this name. Are you sure ? (y/n) \n : ";
                cin >> choice;
                if (choice == 'y')
                    resultFileStr = newStringValue;
            } else {
                resultFileStr = newStringValue;
            }
            break;
        case 's':
        case 'S':
            for (int i = 0; i < numOfInstances;  i++) {
                if (!fileExists(dataFiles[i])) {
                    cout << "File " << dataFiles[i] << " does not exist. Can not start test..";
                    stopTest = true;
                    break;
                }
            }
            if (stopTest)
                break;
            resultFile.open(resultFileStr.c_str());
            if (!resultFile.good()) {
                cout << "\nError with creating file.";
                cout << endl << "Press any key to continue...";
                cin.ignore();
                cin.get();
            } else {
                cout << "Test started. Please wait, it may take a while..." << endl;
                cout << "TESTED FACTOR: " << possibleTestScenario[testScenario] << endl;
                cout << "Number of runs per single scenario : " << repetitionsOfTestCase << endl;
                resultFile << "TESTED FACTOR: " << possibleTestScenario[testScenario] << "\n";
                resultFile << "Number of runs per single scenario : " << repetitionsOfTestCase << " It means that every measurement is average of " << repetitionsOfTestCase << " runs" << "\n";
                for (int i = 0; i<numOfInstances; i++) {

                    cout << "Test for instance: " << dataFiles[i] << "\nLoading...\n";
                    resultFile << ";" << dataFiles[i] << "\n";
                    resultFile << ";;Value of factor; time[ms]; solution; percentage error\n";

                    delete geneticAlgorithm;
                    geneticAlgorithm = new GeneticAlgorithm(dataFiles[i]);

                    for (int k = 0; k<numOfTestedValues; k++) {
                        cout << endl << "\t" << k+1 << " value of " << numOfTestedValues << " : " << valuesOfTestedFactor[k] << endl;
                        resultFile << ";;" << valuesOfTestedFactor[k];
                        time = 0;
                        solution = 0;

                        switch (testScenario) {
                        case 0:
                            geneticAlgorithm->sizeOfPopulation = (int)valuesOfTestedFactor[k];
                            geneticAlgorithm->numberOfChildren = (int)valuesOfTestedFactor[k] / 2;
                            break;
                        case 1:
                            geneticAlgorithm->numberOfPopulations = (int)valuesOfTestedFactor[k];
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

                        for (int j = 1; j <= repetitionsOfTestCase; j++) {
                            cout << "\t\tIteration " << j << " of " << repetitionsOfTestCase << "..." << endl;
                            performanceCountStart = startTimer();
                            solution += geneticAlgorithm->algorithm();
                            performanceCountEnd = endTimer();
                            time += (performanceCountEnd.QuadPart - performanceCountStart.QuadPart);
                        }

                        time /= repetitionsOfTestCase;
                        solution /= repetitionsOfTestCase;
                        time = time / freq.QuadPart * 1000;

                        resultFile << ";" << time;
                        cout << "\tMean time [ms] : " << time << endl;

                        resultFile << ";" << solution;
                        cout << "\tMean solution : " << solution << endl;

                        percentangeError = (solution / (bestKnownSolutions[i] * 1.0) - 1) * 100;
                        cout << "\tMean percentage error = " << percentangeError << endl;
                        resultFile << ";" << percentangeError << "\n";
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

void tabuSearchTestMenu() {
    LARGE_INTEGER performanceCountStart, performanceCountEnd;
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    ofstream resultFile;

    ostringstream tabuTestMenuStream;
    string tabuTestMenu;
    string dataFilesStr;
    string bestKnownSolutionsStr;
    string valuesOfStopCriteriaStr;
    string resultFileStr = getCurrentTime() + ".csv";

    int numOfInstances = 7;

    // nazwy plików z instancjami
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

    // warunek stopu
    int stop_condition = 0;		// 0 - interacje, 1 - time, 2 - not_change
    string stop_condition_string = "Number of iterations";
    // ile powtorzen
    int repetitionsOfTestCase = 3;
    // zmienna na rozwiazanie
    int solution = 0;

    char option;
    bool goBack = false;
    bool setAll = false;
    bool showMenu = true;

    int newValue;
    string newStringValue;
    bool stopTest = false;

    double time = 0;
    double percent = 0;
    int size = tabuSearch->getSize();
    int div_not_change = 20;						// mnozone razy size-  max liczba iteracji bez poprawy rozwiazania do zastosowania dywersyfikacji
    //double alg_time = 1;						// czas dzialania algorytmu
    int num_of_candidates = 2;					// mnozone razy size - liczba kandydatów
    int tabu_length = 10;						// dlugosc listy tabu

    while (!goBack) {
        stopTest = false;
        setAll = false;
        dataFilesStr = arrayToString(dataFiles, numOfInstances);
        bestKnownSolutionsStr = arrayToString(bestKnownSolutions, numOfInstances);
        valuesOfStopCriteriaStr = arrayToString(stopCriteria, how_many_stops);

        tabuTestMenuStream.str("");
        tabuTestMenuStream.clear();
        tabuTestMenuStream <<
                           "|.........................................................................|\n"
                           "|.......... ________________________________ .............................|\n"
                           "|..........|      _  _   _                  |.............................|\n"
                           "|..........|     / |/ | |_ |\\ | | |         |.............................|\n"
                           "|..........|    /     | |_ | \\| |_|         |.............................|\n"
                           "|..._______|________________________________|____________________.........|\n"
                           "|..|                                                             |........|\n"
                           "|..|                   TABU SEARCH                               |........|\n"
                           "|..|                                                             |........|\n"
                           "|..|      OPTION:                                   KEY:         |........|\n"
                           "|..|                                                             |........|\n"
                           "|..|                                                             |........|\n"
                           "|..|      START TEST                                 S           |........|\n"
                           "|..|                                                             |........|\n"
                           "|..|      CHANGE VALUE OF PARAMETER NR. X            X           |........|\n"
                           "|..|                                                             |........|\n"
                           "|..|      SET ALL                                    A           |........|\n"
                           "|..|                                                             |........|\n"
                           "|..|      BACK                                       B           |........|\n"
                           "|..|                                                             |........|\n"
                           "|..|_____________   PARAMETERS OF TEST   ________________________|........|\n"
                           "|..|                                                             |........|\n"
                           "|..|                                                             |........|\n"
                           "|..|   1. NUMBER OF INSTANCES                      " << myfillandwr(' ', 3) << numOfInstances << "           |........|\n"
                           "|..|                                                             |........|\n"
                           "|..|      " << myfillandwl(' ', 49) << dataFilesStr << "      |........|\n"
                           "|..|                                                             |........|\n"
                           "|..|   2. BEST KNOWN SOLUTIONS                                   |........|\n"
                           "|..|                                                             |........|\n"
                           "|..|      " << myfillandwl(' ', 49) << bestKnownSolutionsStr << "      |........|\n"
                           "|..|                                                             |........|\n"
                           "|..|   3. STOPPING CRITERIA                                      |........|\n"
                           "|..|                                                             |........|\n"
                           "|..|      " << myfillandwl(' ', 26) << stop_condition_string << "                             |........|\n"
                           "|..|                                                             |........|\n"
                           "|..|   4. VALUES OF STOPPING CRITERIA                            |........|\n"
                           "|..|                                                             |........|\n"
                           "|..|      " << myfillandwl(' ', 52) << valuesOfStopCriteriaStr << "   |........|\n"
                           "|..|                                                             |........|\n"
                           "|..|   5. REPETITIONS OF TEST CASE                   " << myfillandwl(' ', 3) << repetitionsOfTestCase << "         |........|\n"
                           "|..|                                                             |........|\n"
                           "|..|   6. NAME OF RESULT FILE                                    |........|\n"
                           "|..|                                                             |........|\n"
                           "|..|      " << myfillandwl(' ', 26) << resultFileStr << "                             |........|\n"
                           "|..|_____________________________________________________________|........|\n"
                           "|.........................................................................|\n"
                           "|.........................................................................|\n"
                           "|.........................................................................|\n"
                           "|.........................................................................|\n";


        if (showMenu) {
            system("cls");
            cout << tabuTestMenuStream.str();
        }
        printConsole();
        cin >> option;
        switch (option) {
        case 'b':
        case 'B':
            goBack = true;
            break;
        case 'a':
        case 'A':
            setAll = true;
        case '1':
            cout << "\nInput new value for instances quantity (>0): ";
            cin >> numOfInstances;
            if (numOfInstances <= 0)
                break;
            cout << "Provide names of data files (.atsp) : ";
            for (int i = 1; i <= numOfInstances; i++) {
                cout << endl << i << " of " << numOfInstances << " : ";
                cin >> newStringValue;
                if (newStringValue.find(".atsp") != string::npos) {
                } else {
                    newStringValue += ".atsp";
                }

                if (!fileExists(newStringValue)) {
                    cout << endl << "There is no such file in directory, try again.";
                    i--;
                } else {
                    dataFiles[i - 1] = newStringValue;
                }
            }
            if (!setAll)
                break;
        case '2':
            cout << "Provide best known solutions for these instances." << endl;
            for (int i = 1; i <= numOfInstances; i++) {
                cout << endl << i << " of " << numOfInstances << " : ";
                cin >> bestKnownSolutionsStr[i - 1];
            }
            if (!setAll)
                break;
        case '3':
            cout << "Choose stopping criteria:\n0 - number of iterations, 1 - time, 2 - not_change\n\nYour choice : ";
            cin >> newValue;
            if (newValue >= 0 && newValue <= 2) stop_condition = newValue;
            if (stop_condition == 0) stop_condition_string = "Number of iterations";
            else if (stop_condition == 1) stop_condition_string = "Time";
            else if (stop_condition == 2) stop_condition_string = "No change";
            if (!setAll)
                break;
        case '4':
            cout << "How many values of stopping criteria do you want to test ? \n";
            cin >> how_many_stops;
            cout << "Input values:\n";
            for (int i = 0; i<how_many_stops; i++) {
                cout << i << " : ";
                cin >> stopCriteria[i];
                cout << endl;
            }
            if (!setAll)
                break;
        case '5':
            do {
                cout << "Insert number of repetitions of a test case (>0) : ";
                cin >> repetitionsOfTestCase;
            } while (repetitionsOfTestCase <= 0);
            if (!setAll)
                break;
        case '6':
            do {
                cout << endl << "Input name of result file (.csv): ";
                cin >> newStringValue;
            } while (resultFileStr.empty());

            if (newStringValue.find(".csv") != string::npos) {
            } else {
                newStringValue += ".csv";
            }
            if (fileExists(newStringValue)) {
                cout << "There is a file with this name. Are you sure ? (y/n) \n : ";
                cin >> option;
                if (option == 'y')
                    resultFileStr = newStringValue;
            } else {
                resultFileStr = newStringValue;
            }
            if (!setAll)
                break;

        case 's':
        case 'S':
            for (int i = 0; i < numOfInstances; i++) {
                if (!fileExists(dataFiles[i])) {
                    cout << "File " << dataFiles[i] << " does not exist. Can not start test..";
                    stopTest = true;
                    break;
                }
            }
            if (stopTest)
                break;
            resultFile.open(resultFileStr.c_str());
            if (resultFile.good()) {
                for (int i = 0; i<numOfInstances; i++) {
                    cout << "Test for file: " << dataFiles[i] << "\nReading...\n";
                    resultFile << dataFiles[i] << endl;
                    delete tabuSearch;
                    tabuSearch = new TabuSearch(dataFiles[i]);
                    size = tabuSearch->getSize();
                    cout << "Stopping criteria : ";
                    resultFile << "Stopping criteria : ";
                    if (stop_condition == 0) cout << " - number of itarations.\n";
                    else if (stop_condition == 1) cout << " - time.\n";
                    else if (stop_condition == 2) cout << " - no change.\n";
                    if (stop_condition == 0) resultFile << " - - number of itarations.\n";
                    else if (stop_condition == 1) resultFile << " - time.\n";
                    else if (stop_condition == 2) resultFile << " - no change.\n";
                    resultFile << "WITH DIVERSIFICATION" << endl;
                    for (int k = 0; k<how_many_stops; k++) {
                        cout << "STOP AT: " << stopCriteria[k] << endl;
                        resultFile.width(12);
                        resultFile << stopCriteria[k];
                        time = 0;
                        solution = 0;
                        if (stop_condition == 0)
                            tabuSearch->setParameters((int)(stopCriteria[k]), -1, div_not_change*size, -1, num_of_candidates*size, tabu_length, true, 0);
                        else if (stop_condition == 1)
                            tabuSearch->setParameters(-1, -1, div_not_change*size, (int)(stopCriteria[k]), num_of_candidates*size, tabu_length, true, 1);
                        else if (stop_condition == 2)
                            tabuSearch->setParameters(-1, (int)(stopCriteria[k]), div_not_change*size, -1, num_of_candidates*size, tabu_length, true, 2);
                        for (int j = 1; j <= repetitionsOfTestCase; j++) {
                            cout << "REPEAT with diversification : " << j << endl;
                            performanceCountStart = startTimer();
                            solution += tabuSearch->algorithm();
                            performanceCountEnd = endTimer();
                            time += (performanceCountEnd.QuadPart - performanceCountStart.QuadPart);
                        }
                        time /= repetitionsOfTestCase;
                        solution /= repetitionsOfTestCase;
                        time = time / freq.QuadPart * 1000;
                        if (stop_condition != 1) {
                            resultFile.width(13);
                            resultFile << time;
                        }
                        cout << "Average time = " << time << endl;
                        resultFile.width(13);
                        resultFile << solution;
                        cout << "Average solution = " << solution << endl;
                        percent = (solution / (bestKnownSolutions[i] * 1.0) - 1) * 100;
                        cout << "Average percentage error = " << percent << endl;
                        resultFile.width(13);
                        resultFile << percent << endl;
                    }
                    resultFile << "WITHOUT DIVERSIFICATION" << endl;
                    for (int k = 0; k<how_many_stops; k++) {
                        cout << "STOP AT: " << stopCriteria[k] << endl;
                        resultFile.width(12);
                        resultFile << stopCriteria[k];
                        time = 0;
                        solution = 0;
                        if (stop_condition == 0)
                            tabuSearch->setParameters((int)stopCriteria[k], -1, div_not_change*size, -1, num_of_candidates*size, tabu_length, false, 0);
                        else if (stop_condition == 1)
                            tabuSearch->setParameters(-1, -1, div_not_change*size, (int)stopCriteria[k], num_of_candidates*size, tabu_length, false, 1);
                        else if (stop_condition == 2)
                            tabuSearch->setParameters(-1, (int)stopCriteria[k], div_not_change*size, -1, num_of_candidates*size, tabu_length, false, 2);
                        for (int j = 1; j <= repetitionsOfTestCase; j++) {
                            cout << "REPEAT without diversification : " << j << endl;
                            performanceCountStart = startTimer();
                            solution += tabuSearch->algorithm();
                            performanceCountEnd = endTimer();
                            time += (performanceCountEnd.QuadPart - performanceCountStart.QuadPart);
                        }
                        time /= repetitionsOfTestCase;
                        solution /= repetitionsOfTestCase;
                        time = time / freq.QuadPart * 1000;
                        if (stop_condition != 1) {
                            resultFile.width(13);
                            resultFile << time;
                        }
                        cout << "Average time = " << time << endl;
                        resultFile.width(13);
                        resultFile << solution;
                        cout << "Average solution = " << solution << endl;
                        percent = (solution / (bestKnownSolutions[i] * 1.0) - 1) * 100;
                        cout << "Average percentage error = " << percent << endl;
                        resultFile.width(13);
                        resultFile << percent << endl;
                    }

                }
                cout << endl << "Press a key to continue...";
                cin.ignore();
                cin.get();
                resultFile.close();
            } else cout << "\nResult file error!";
            break;
        default:
            break;
        }
    }
}
