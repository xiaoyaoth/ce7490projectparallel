#ifndef STATE_H_
#define STATE_H_

#include "Base.h"
#include <string>

class State{
private:
	float time;
	Base *blist;
	int baseAmount;
public:
	State(float t, Base *b, int bs);
	string toString();
};

#endif