#pragma once
#include "Move.h"
#include <iostream>

class TabuList
{
private:
	int length;
	Move * moves;
	unsigned int pointer;

public:
	void add(Move* move);
	bool isAllowed(const int* position, Move* move);

	int getLength(){ return length; };
	void showTabu();

	TabuList(int l);
	TabuList();
	~TabuList();
};


