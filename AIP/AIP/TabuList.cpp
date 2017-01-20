#include "stdafx.h"
#include "TabuList.h"

using namespace std;

TabuList::TabuList(int l) {
    this->length = l;
    this->pointer = 0;
    this->moves = new Move[l]();
}

void TabuList::add(Move* move) {
    moves[pointer%length].setBegin(move->getBegin());
    moves[pointer%length].setEnd(move->getEnd());
    delete move;
    pointer++;
}

bool TabuList::isAllowed(const int* position, Move* move) {
    for (int i = 0; i < length; i++) {
        if (moves[i].getBegin() == position[move->getBegin()] && moves[i].getEnd() == position[move->getEnd()]) {
            delete move;
            return false;
        }
    }
    delete move;
    return true;
}

void TabuList::showTabu() {
    cout << endl << "---------TABU LIST------------" << endl;
    for (int i = 0; i < length; i++) {
        cout << moves[i].getBegin() << "   " << moves[i].getEnd() << endl;
    }
    cout << "----------------------------------" << endl;
}

TabuList::TabuList() {
}

TabuList::~TabuList() {
    length = 0;
    delete[] moves;
    pointer = 0;
}
