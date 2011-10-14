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

Process::Process(int pno, int rank, MPI_Datatype t){
	mpiType = t;
	eventAmount = 0;
	queue.empty();

	pid = rank;
	procAmount = pno;
	baseAmount = BASENO/procAmount+1;
	blist = new Base[baseAmount];
	for(int i = 0; i<baseAmount; i++){
		int bid = rank*baseAmount + i;
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
	Event * e = queue.top();
	queue.pop();
	return e;
}

int Process::getQueueSize(){
	return queue.size();
}

void Process::run(){
	MPI_Request req;
	int flag = true;
	int stat;
	string rec; //one record
	eventStruct e;
	ifstream fin;
	ofstream fout("out.txt");

	int i = 0;
	bool fini = false; //current process fini
	bool prevFini = false; // previous process has finished;

	if(pid == 0){
		fin.open("C:\\Users\\xli15\\Documents\\Visual Studio 2010\\Projects\\ParallelSimulator\\ParallelSimulator\\data.txt");
		if(!fin)
			cout<<"file not exist"<<endl;
	}

	while(!fini){
		if(pid == 0 && !fin.eof() && ++i<10){
			getline(fin, rec);
			e =  parseData(rec);
			if(e.getBaseID()<baseAmount){
				this->insert(new CallInitiationEvent(e));
			}
			else{
				int dest = e.getBaseID()/baseAmount;
				sendEvent(e, dest);
			}
		}
		stat = receiveEvent(&req, &flag);
		if(stat == FINI){
			cout<<"stat = FINI"<<endl;
			prevFini = true;
		}
		if(prevFini == true && queue.size() == 0){
			cout<<"prevFini = true && queue.size() == 0"<<endl;
			fini = true;
			e.etype = FINI;
			sendEvent(e, pid+1);
			break; // no event anymore, execution finished;
		}
		Event * cur = getNextEvent();
		cout<<cur->toString()<<endl;
	}
	cout<<"finish run"<<endl;
}

void Process::sendEvent(eventStruct e, int dest){
	if(dest<procAmount)
		MPI_Send(&e, 1, mpiType, dest, 99, MPI_COMM_WORLD);
	else
		cout<<"no more proc"<<endl;
}

int Process::receiveEvent(MPI_Request *req,	int *flag){
	MPI_Status status;
	eventStruct e;
	if(*flag == true)
		MPI_Irecv(&e, 1, mpiType, MPI_ANY_SOURCE, 99, MPI_COMM_WORLD, req);
	MPI_Test(req, flag, &status); // through debugging I found when this line is past, it directly went to finalize
	if(*flag == true){
		if(e.etype == FINI)
			return FINI;
		else if(e.etype == HANDO)
			this->insert(new CallHandoverEvent(e));
		else if(e.etype == INIT)
			this->insert(new CallInitiationEvent(e));
		else if(e.etype == TERMI)
			this->insert(new CallTerminationEvent(e));
		return 0;
	}else 
		cout<<"read fail"<<endl; 
	return 0;
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
