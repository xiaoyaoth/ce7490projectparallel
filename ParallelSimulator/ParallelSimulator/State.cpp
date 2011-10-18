#include "State.h"
#include <sstream>

State::State(float t, Base * b, int bn){
	time = t;
	blist = new Base[bn];
	for(int i = 0; i<bn; i++)
		blist[i] = b[i];
	baseAmount = bn;
}

string State::toString(){
	stringstream ss;
	ss<<time<<"_";
	for(int i = 0; i<baseAmount; i++)
		ss<<blist[i].toString()<<" ";
	return ss.str();
}