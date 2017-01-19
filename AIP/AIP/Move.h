#pragma once
class Move {
private:
    int begin;
    int end;

public:
    void setBegin(int a) {
        begin = a;
    };
    void setEnd(int a) {
        end = a;
    };
    int getBegin() {
        return begin;
    };
    int getEnd() {
        return end;
    };

    Move();
    Move(int, int);

    ~Move();
};

