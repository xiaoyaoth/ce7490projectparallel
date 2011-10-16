#ifndef PROCESS_H_
#define PROCESS_H_

#include "mpi.h"
#include "Base.h"
#include "Event.h"
#include <queue>
#include <list>

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
	int procAmount;

	MPI_Datatype mpiType;

	priority_queue<Event*, vector<Event*>, comp> queue;
	list<struct eventStruct> sendList;
public:
	/*initiation*/
	Process(int procAmount, int myRank, MPI_Datatype t);

	/*queue operation*/
	void insert(Event * e);
	Event* getNextEvent();
	int getQueueSize();

	/*main logic for each process*/
	void run();

	/*MPI operation*/
	void sendEvent();
	int receiveEvent();
	bool receiveACK();
};

#endif;