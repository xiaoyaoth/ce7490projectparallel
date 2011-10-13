#include "Process.h"
#include "CallHandoverEvent.h"
#include "CallInitiationEvent.h"
#include "CallTerminationEvent.h"
#include <fstream>
#include <string>

using namespace std;
struct eventStruct toHandoverStruct(int arrivalNo, float duration, float pos, bool prevCallReserved, float speed, float time);
struct eventStruct toInitiationStruct(int arrivalNo, float duration, float pos, float speed, float time);
struct eventStruct toTerminationStruct(int arrivalNo, float pos, bool prevCallReserved, float time);
struct eventStruct parseData(string rec);

Process::Process(int procAmount, int myRank, MPI_Datatype t){
	mpiType = t;
	eventAmount = 0;
	queue.empty();
	pid = myRank;
	baseAmount = BASENO/procAmount+1;
	blist = new Base[baseAmount];
	for(int i = 0; i<baseAmount; i++){
		int bid = myRank*baseAmount + i;
		if(bid<20)
			blist[i].setBaseID(bid);
		else
			blist[i].setBaseID(-1);
		cout<<blist[i].getBaseID()<<endl;
	}
}

void Process::insert(Event * e){
	eventAmount++;
	queue.push(e);
}

Event * Process::getNextEvent(){
	return queue.top();
}

void Process::run(){
	MPI_Request req;
	int i = 0;
	if(pid == 0){
		ifstream fin;
		fin.open("C:\\Users\\xli15\\Documents\\Visual Studio 2010\\Projects\\ParallelSimulator\\ParallelSimulator\\data.txt");
		ofstream fout("out.txt");
		if(!fin)
			cout<<"file not exist"<<endl;
		string rec; //one record
		eventStruct e;
		while(!fin.eof() && ++i<10){
			getline(fin, rec);
			e =  parseData(rec);
			if(e.getBaseID()<baseAmount){
				this->insert(new CallInitiationEvent(e));
				e.toString();
			}
			else
				sendEvent(e);
		}
	}else
		while(true)
			receiveEvent(req);
}

void Process::sendEvent(eventStruct e){
	int destPid = e.getBaseID()/baseAmount;
	MPI_Send(&e, 1, mpiType, destPid, 99, MPI_COMM_WORLD);
}

void Process::receiveEvent(MPI_Request &req){
	MPI_Status status;
	eventStruct e;
	MPI_Irecv(&e, 1, mpiType, MPI_ANY_SOURCE, 99, MPI_COMM_WORLD, &req);
	MPI_Wait(&req, &status);
	if(e.etype == 1)
		this->insert(new CallHandoverEvent(e));
	else if(e.etype = 0)
		this->insert(new CallInitiationEvent(e));
	else
		this->insert(new CallTerminationEvent(e));
	e.toString();
	cout<<"finish"<<endl; 
	//how to confirm if the simulation finish running? because you don't know by when you will not receive any message
	//and another thing, if receive is blocked, the following stuff cannot be done? if multi-thread not escapable?
}

struct eventStruct toHandoverStruct(int arrivalNo, float duration, float pos, bool prevCallReserved, float speed, float time){
	eventStruct e;
	e.ano = arrivalNo;
	e.dura = duration;
	e.etype = 1;
	e.pos = pos;
	e.rc = (int)prevCallReserved;
	e.speed = speed;
	e.time = time;
	return e;
}

struct eventStruct toTerminationStruct(int arrivalNo, float pos, bool prevCallReserved, float time){
	eventStruct e;
	e.ano = arrivalNo;
	e.dura = -1;
	e.etype = 2;
	e.pos = pos;
	e.rc = prevCallReserved;
	e.speed = -1;
	e.time = time;
	return e;
}

struct eventStruct toInitiationStruct(int arrivalNo, float duration, float pos, float speed, float time){
	eventStruct e;
	e.ano = arrivalNo;
	e.dura = duration;
	e.etype = 0;
	e.pos = pos;
	e.rc = 0;
	e.speed = speed;
	e.time = time;
	return e;
}

struct eventStruct parseData(string rec){
	char * cstr, *p;
	int no, baseID;
	float time, duration, speed, pos;

	cstr = new char[rec.size()+1];
	strcpy_s(cstr, rec.size()+1, rec.c_str());

	p=strtok (cstr,"\t");
	no = atoi(p);
	p=strtok(NULL,"\t");
	time = (float)atof(p);
	p=strtok(NULL,"\t");
	baseID = atoi(p) - 1;
	pos = baseID*2 + 1;
	p=strtok(NULL,"\t");
	duration = (float)atof(p);
	p=strtok(NULL,"\t");
	speed = (float)atof(p);

	eventStruct e;
	e.ano = no;
	e.dura = duration;
	e.etype = 0;
	e.pos = pos;
	e.rc = 0;
	e.speed = speed;
	e.time = time;

	return e;
}
