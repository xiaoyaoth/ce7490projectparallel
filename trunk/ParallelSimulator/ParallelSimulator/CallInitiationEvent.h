#ifndef CALLINITIATIONEVENT_H_
#define CALLINITIATIONEVENT_H_

#include "Event.h"

class CallInitiationEvent : public Event
{
private:
	float speed;
	float duration;
public:
	CallInitiationEvent(float time, float speed, float position, float duration, int no);
	CallInitiationEvent(struct eventStruct e);
	void handleEvent(Base blist[]);
	void scheme0(Base blist[]);
	void scheme1(Base blist[]);
	string toString();
};
#endif