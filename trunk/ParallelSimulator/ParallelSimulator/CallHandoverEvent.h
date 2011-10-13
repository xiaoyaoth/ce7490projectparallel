#ifndef CALLHANDOVEREVENT_H_
#define CALLHANDOVEREVENT_H_

#include "Event.h"
#include "CallTerminationEvent.h"

class CallHandoverEvent : public Event
{
private:
	float speed;
	float duration;
	bool prevCallReserved; // added for the second scheme
public:
	CallHandoverEvent(float t, float s, int bid, float d, int ano);
	CallHandoverEvent(float t, float s, int bid, float d, int ano, bool rc);// added for the second scheme
	void handleEvent(Base blist[]);
	void scheme0(Base blist[]);
	void scheme1(Base blist[]);
	string getOutput(Base blist[]);

};

#endif