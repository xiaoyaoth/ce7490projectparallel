#ifndef PROCESS_H_
#define PROCESS_H_

#include "mpi.h"
#include "Base.h"
#include "Event.h"
#include <queue>

struct comp{
	bool operator() (Event *e1, Event *e2){
		return e1->getTime()>e2->getTime();
	}
};

class Process{
private:
	Base * blist;
	int pid;
	int baseAmount;
	int eventAmount;

	MPI_Datatype mpiType;

	priority_queue<Event*, vector<Event*>, comp> queue;
public:
	Process(int procAmount, int myRank, MPI_Datatype t);
	void insert(Event * e);
	Event* getNextEvent();
	void run();
	void sendEvent(eventStruct e);
	void receiveEvent(MPI_Request &req);
};

#endif;