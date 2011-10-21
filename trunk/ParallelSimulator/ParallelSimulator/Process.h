#ifndef PROCESS_H_
#define PROCESS_H_

#include "mpi.h"
#include "Base.h"
#include "Event.h"
#include <queue>
#include <list>
#include <fstream>

struct comp{
	bool operator() (Event *e1, Event *e2){
		return e1->getTime()>e2->getTime();
	}
};

struct comp2{
	bool operator() (eventStruct e1, eventStruct e2){
		return e1.time>e2.time;
	}
};

class Process{
private:
	/*logic*/
	Base * blist;	

	/*MPI*/
	static int baseAmount; // base amount in every process
	static int procAmount; // total process amounts
	static int pid;
	MPI_Datatype mpiType;
	MPI_Request sendReq;
	MPI_Request recvReq;
	int sendFlag;
	int recvFlag;
	static priority_queue<struct eventStruct, vector<struct eventStruct>, comp2> sendList;
	//static list<struct eventStruct> sendList;
	static priority_queue<Event*, vector<Event*>, comp> initQueue;
	static priority_queue<Event*, vector<Event*>, comp> handQueue;
	struct eventStruct recvElem;
	static float procTime;
	static bool prevFini;

public:
	/*initiation*/
	Process(int procAmount, int myRank, MPI_Datatype t);

	/*queue operation*/
	static void insert(Event * e);
	static Event* getNextEvent();
	static int getQueueSize();

	/*main logic for each process*/
	void run();
	struct eventStruct parseData(string rec);

	/*MPI operation*/
	void sendMessage();
	int recvMessage();
	static int getBaseAmount();
	static int getProcAmount();
	static int getPid();
	static void insertSendList(struct eventStruct e);
	bool queuesClean();
};

#endif;