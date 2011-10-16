#ifndef EVENT_H_
#define EVENT_H_

#ifndef NULL
#define NULL 0
#endif

#define DIAMETER 2
#define SCHEME 1

#define INIT 0
#define HANDO 1
#define TERMI 2
#define FINI 3

#define INTNO 4
#define FLOATNO 4

#include <iostream>
#include <sstream>
#include "Base.h"

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
	virtual string toString();
	static string getResult();
};

struct eventStruct{
	/*parameter related MPI*/
	int etype;//event type, 0 is initial, 1 is handover, 2 is termination

	/*parameter related logic*/
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
		return (int)pos/DIAMETER;
	}
};

#endif