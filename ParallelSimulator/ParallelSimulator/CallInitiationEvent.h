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
public:
	CallInitiationEvent(float time, float speed, float position, float duration, int no);
	CallInitiationEvent(eventStruct e);
	//void handleEvent(Base blist[]);
	//void scheme0(Base blist[]);
	//void scheme1(Base blist[]);
	//string getOutput(Base blist[]);
};
#endif