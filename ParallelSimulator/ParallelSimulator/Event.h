#ifndef EVENT_H_
#define EVENT_H_

#ifndef NULL
#define NULL 0
#endif

#ifndef DIAMETER
#define DIAMETER 2
#endif

#ifndef POS
#define POS 1
#endif

#ifndef SCHEME
#define SCHEME 1
#endif

#include <iostream>
#include <sstream>
//#include "Base.h"

using namespace std;

class Event{

protected:

	/*Event attribute*/
	float time;
	float position;
	int arrivalNo;
	Event * nextEvent;
	//int eid;

	static int counter;

	/*statistics, declared as static*/
	static int block; // count for call-block
	static int drop; // count of call-drop
	static int success; //count of handover
	static int total; //total event amount

public:

	Event();
	Event(float time, float pos, int ano);
	float getTime();
	int getBaseID();
	Event * getNextEventPtr();
	void setNextEventPtr(Event * e);
	int getEventID();
	int getArrivalNo();

	//virtual void handleEvent(Base blist[]);
	//virtual string getOutput(Base blist[]);

	static string getResult();
};


struct eventStruct{
	int etype;//event type, 0 is initial, 1 is handover, 2 is termination
	int ano;
	int rc;
	float time;
	float speed;
	float dura;
	float pos;

	void toString(){
		cout<<"type:"<<etype<<"\tano:"<<ano<<"\tdura:"<<dura
			<<"\tbid:"<<getBaseID()<<"\trc:"<<rc<<"\tspeed:"<<speed
			<<"\ttime:"<<time<<endl;
	}

	int getBaseID(){
		return pos/2;
	}
};

#endif