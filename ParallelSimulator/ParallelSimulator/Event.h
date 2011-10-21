#ifndef EVENT_H_
#define EVENT_H_

#ifndef NULL
#define NULL 0
#endif

#define DIAMETER 2
#define SCHEME 0

#define INIT 0
#define HANDO 1
#define TERMI 2
#define FINI 3
#define INITFINI 4

#define INTNO 4
#define FLOATNO 4

#include <sstream>
#include "Base.h"

class Event{

protected:

	/*Event attribute*/
	float time;
	int baseId;
	int arrivalNo;

	/*statistics, declared as static*/
	static int block; // count for call-block
	static int drop; // count of call-drop
	static int success; //count of handover

public:
	/*logic*/
	Event();
	Event(float time, int bid, int ano);
	float getTime();
	int getBaseID();
	Event * getNextEventPtr();
	void setNextEventPtr(Event * e);
	int getEventID();
	int getArrivalNo();

	virtual void handleEvent(Base blist[]);
	virtual string toString();
	static string getResult();

	/*MPI*/
	void insertIntoEventQueue(Event * e);
	void insertIntoSendList(struct eventStruct e);
	int getBlistIndex();
	int getBlistSize();

	struct eventStruct Event::toInitiationStruct(int ano, float dura, float pos, float speed, float time);
	struct eventStruct Event::toHandoverStruct(int ano, float dura, int bid, bool rc, float speed, float time);
	struct eventStruct Event::toTerminationStruct(int ano, int bid, bool rc, float time);	
};

struct eventStruct{
	/*parameter related MPI*/
	int etype;//event type, 0 is initial, 1 is handover, 2 is termination

	/*parameter related logic*/
	int ano;
	int rc;
	int bid;
	float time;
	float speed;
	float dura;
	float posInBase;

	string toString(){
		stringstream ss;
		ss<<"type:"<<etype<<"\tano:"<<ano<<"\tdura:"<<dura
			<<"\tbid:"<<bid<<"\tposInBase:"<<posInBase<<"\trc:"
			<<rc<<"\tspeed:"<<speed<<"\ttime:"<<time<<endl;
		return ss.str();
	}
};

#endif