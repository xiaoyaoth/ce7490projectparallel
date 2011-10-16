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
	/*logic*/
	Base * blist;	
	priority_queue<Event*, vector<Event*>, comp> queue;

	/*MPI*/
	static int baseAmount; // base amount in every process
	static int procAmount; // total process amounts
	int pid;
	MPI_Datatype mpiType;
	MPI_Request sendReq;
	MPI_Request recvReq;
	int sendFlag;
	int recvFlag;
	list<struct eventStruct> sendList;
	struct eventStruct recvElem;

public:
	/*initiation*/
	Process(int procAmount, int myRank, MPI_Datatype t);

	/*queue operation*/
	void insert(Event * e);
	Event* getNextEvent();
	int getQueueSize();

	/*main logic for each process*/
	void run();
	struct eventStruct parseData(string rec);

	/*MPI operation*/
	void sendMessage();
	int recvMessage();
	static int getBaseAmount();
	static int getProcAmount();
};

#endif;