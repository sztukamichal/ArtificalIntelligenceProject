#pragma once
// Klasa reprezentuje ruchy miast w permutacjach
class Move
{
private:
	int begin;
	int end;

public:

	Move();
	Move(int, int);
	void setBegin(int a){ begin = a; };
	void setEnd(int a){ end = a; };
	int getBegin(){ return begin; };
	int getEnd() { return end; };
	~Move();
};

