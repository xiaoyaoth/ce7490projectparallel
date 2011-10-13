#ifndef CALLTERMINATIONEVENT_H_
#define CALLTERMINATIONEVENT_H_

#include "Event.h"

class CallTerminationEvent : public Event
{
private:
	bool prevCallReserved; // added for the second scheme;
public:
	CallTerminationEvent(float t, int bid, int ano);
	CallTerminationEvent(float t, int bid, int ano, bool rc); // added for the second scheme;
	void handleEvent(Base blist[]);
	void scheme0(Base blist[]);
	void scheme1(Base blist[]);
	string getOutput(Base blist[]);
};
#endif