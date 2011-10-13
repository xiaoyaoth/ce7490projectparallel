#ifndef CALLINITIATIONEVENT_H_
#define CALLINITIATIONEVENT_H_

#include "Event.h"
#include "CallHandoverEvent.h"
#include "CallTerminationEvent.h"

class CallInitiationEvent : public Event
{
private:
	float speed;
	float duration;
	float position;
public:
	CallInitiationEvent(float time, float speed, float position, float duration, int no);
	//void handleEvent(Base blist[]);
	//void scheme0(Base blist[]);
	//void scheme1(Base blist[]);
	//string getOutput(Base blist[]);
};
#endif