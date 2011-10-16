#include "Process.h"
#include "CallHandoverEvent.h"
#include "CallInitiationEvent.h"
#include "CallTerminationEvent.h"
#include <fstream>
#include <string>

using namespace std;

Process::Process(int pno, int rank, MPI_Datatype t){
	mpiType = t;
	sendFlag = 1;
	recvFlag = 1;
	ack = true;

	/*logic*/
	pid = rank;
	eventAmount = 0;
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
	queue.empty();
}

void Process::insert(Event * e){
	eventAmount++;
	queue.push(e);
}

Event * Process::getNextEvent(){
	Event * e;
	if(queue.size() != 0){
		e = queue.top();
		queue.pop();
	} else
		e = NULL;
	return e;
}

int Process::getQueueSize(){
	return queue.size();
}

void Process::run(){
	int stat = 3;
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
				sendList.push_back(e);
			}
		}
		sendMessage();
		//stat = receiveEvent();
		if(stat == FINI){
			cout<<"stat = FINI"<<endl;
			prevFini = true;
		}
		if(prevFini == true && queue.size() == 0){
			cout<<"prevFini = true && queue.size() == 0"<<endl;
			fini = true;
			e.etype = FINI;
			//sendEvent(e, pid+1);
			break; // no event anymore, execution finished;
		}
		Event * cur = getNextEvent();
		if(cur != NULL)
			cout<<cur->toString()<<endl;
	}
	cout<<"finish run"<<endl;
}

/*Events are inserted into back, ACK are inserted into front, 
  every pop operation is done from the front*/
void Process::sendMessage(){
	MPI_Status stat;
	if(sendFlag == true && ack == true){
		struct eventStruct e = sendList.front();
		MPI_Isend(&e, 1, mpiType, e.dest, 99, MPI_COMM_WORLD, &sendReq);
	} else if(sendFlag == false && ack == true){
		MPI_Test(&sendReq, &sendFlag, &stat);
	} else;// ACK is false
}

int Process::recvMessage(){
	MPI_Status stat;
	eventStruct e;

	if(recvFlag == true)
		MPI_Irecv(&e, 1, mpiType, pid-1, 99, MPI_COMM_WORLD, &recvReq);
	else
		MPI_Test(&req, &recvFlag, &stat);
	e.toString();
	if(e.etype == FINI)
		return FINI;
	else if(e.etype == HANDO)
		this->insert(new CallHandoverEvent(e));
	else if(e.etype == INIT)
		this->insert(new CallInitiationEvent(e));
	else if(e.etype == TERMI)
		this->insert(new CallTerminationEvent(e));
	return 0;
}

struct eventStruct Process::parseData(string rec){
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
	pos = (float)baseID*2 + 1;
	p=strtok(NULL,"\t");
	duration = (float)atof(p);
	p=strtok(NULL,"\t");
	speed = (float)atof(p);

	eventStruct e;
	e.etype = 0;
	e.dest = e.getBaseID()/baseAmount;
	e.ano = no;
	e.dura = duration;
	e.pos = pos;
	e.rc = 0;
	e.speed = speed;
	e.time = time;

	return e;
}
