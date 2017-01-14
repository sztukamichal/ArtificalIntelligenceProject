#include "stdafx.h"
#include "Move.h"


Move::~Move() {
	begin = -1;
	end = -1;
}

Move::Move() {
	begin = -1;
	end = -1;
}

Move::Move(int begin, int end) {
	this->begin = begin;
	this->end = end;
}
