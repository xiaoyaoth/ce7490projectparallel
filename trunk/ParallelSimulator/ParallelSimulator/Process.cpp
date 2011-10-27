#include "Process.h"
#include "CallHandoverEvent.h"
#include "CallInitiationEvent.h"
#include "CallTerminationEvent.h"
#include <string>
#include <iostream>
#include <sstream>

#define FREQ 100 // state saving freqency

int Process::baseAmount = 0;
int Process::procAmount = 0;
int Process::pid = 0;
float Process::procTime;
priority_queue<Event*, vector<Event*>, comp> Process::initQueue;
priority_queue<Event*, vector<Event*>, comp> Process::handQueue;
priority_queue<struct eventStruct, vector<struct eventStruct>, comp2> Process::sendList;
bool Process::prevFini = false;

using namespace std;

Process::Process(int pno, int rank, MPI_Datatype t){
	/*MPI*/
	pid = rank;
	procAmount = pno;
	baseAmount = BASENO/procAmount+1;
	mpiType = t;
	sendFlag = 1;
	recvFlag = 1;
	sendReq = MPI_REQUEST_NULL; // the default of MPI_Request is MPI_REQUEST_NULL
	recvReq = MPI_REQUEST_NULL;

	/*logic*/
	blist = new Base[baseAmount];
	for(int i = 0; i<baseAmount; i++){
		int bid = rank*baseAmount + i;
		if(bid<20)
			blist[i].setBaseID(bid);
		else
			blist[i].setBaseID(-1);
		cout<<blist[i].getBaseID()<<endl;
	}
	procTime = 0;

	/*io*/
	stringstream ss;
	ss<<pid<<".txt";
	fout.open(ss.str().c_str());
}

void Process::insert(Event * e){
	initQueue.push(e);
}

Event * Process::getNextEvent(){
	Event * e;
	int hs = handQueue.size();
	int is = initQueue.size();
	//	cout<<"initQueue:"<<is<<" handQueue:"<<hs;
	//if(queue.size() != 0){
	//	e = queue.top();
	//	queue.pop();
	//} else
	//	e = NULL;
	if(pid != 0){
		if(hs != 0 && is != 0){
			Event * e1 = handQueue.top();
			Event * e2 = initQueue.top();
			if(e1->getTime()<e2->getTime()){
				e = e1;
				handQueue.pop();
			} else {
				e = e2;
				initQueue.pop();
			}
		}else if (prevFini == true && hs+is != 0){
			if(hs > 0){
				e = handQueue.top();
				handQueue.pop();
			} else {
				e = initQueue.top();
				initQueue.pop();
			}
		} else
			e = NULL;
	} else {
		if(is > 0){
			e = initQueue.top();
			initQueue.pop();
		} else {
			prevFini = true; /*determine if Proc0 is prevfini*/
			e = NULL;
		}
	}
	return e;
}

int Process::getQueueSize(){
	return -1;
}

struct eventStruct parseStocha(string rec){
	char * cstr, *p;
	int no, baseID;
	float time, duration, speed, pos;

	cstr = new char[rec.size()+1];
	strcpy(cstr,rec.c_str());

	p=strtok (cstr,"\t");
	no = atoi(p);
	p=strtok(NULL,"\t");
	time = (float)atof(p);
	p=strtok(NULL,"\t");
	duration = (float)atof(p);
	p=strtok(NULL,"\t");
	pos = atof(p);
	p=strtok(NULL,"\t");
	speed = (float)atof(p);

	struct eventStruct e;
	e.etype = 0;
	e.ano = no;
	e.dura = duration;
	e.bid = (int)pos/DIAMETER;
	e.posInBase = pos-e.bid*DIAMETER;
	e.rc = 0;
	e.speed = speed;
	e.time = time;

	return e;
}

struct eventStruct parseDeterm(string rec){
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

	struct eventStruct e;
	e.etype = 0;
	e.ano = no;
	e.dura = duration;
	e.bid = pos/DIAMETER;
	e.posInBase = pos-e.bid*DIAMETER;
	e.rc = 0;
	e.speed = speed;
	e.time = time;

	return e;
}

struct eventStruct Process::parseData(string rec){
	bool determine = false;
	if(determine)
		return parseDeterm(rec);
	else
		return parseStocha(rec);
}

void Process::initialize(){
	string rec; //one record
	ifstream fin;
	MPI_Status stat;
	if(pid == 0){
		fin.open("data.txt.10w");
		if(!fin)
			cout<<"file not exist"<<endl;
		while(!fin.eof()){
			struct eventStruct e;
			getline(fin, rec);
			e =  parseData(rec);
			int dest = (int)e.bid/baseAmount;
			if(e.bid<baseAmount)
				insert(new CallInitiationEvent(e));
			else
				MPI_Send(&e, 1, mpiType, dest, 99, MPI_COMM_WORLD);
		}
		for(int i = 1; i<procAmount; i++){
			struct eventStruct e;
			e.etype = INITFINI;
			e.bid = i;
			e.time = 1000000;
			MPI_Send(&e, 1, mpiType, e.bid, 99, MPI_COMM_WORLD);
		}
	} else{
		while(true){
			struct eventStruct e;
			MPI_Recv(&e, 1, mpiType, 0, 99, MPI_COMM_WORLD, &stat);
			if(e.etype == INIT)
				initQueue.push(new CallInitiationEvent(e));
			else if(e.etype == INITFINI)
				break;
			else
				cout<<"initialize wrong"<<e.toString();
		}
		cout<<"init recv fini"<<endl;
	}
}

int Process::getBaseAmount(){
	return baseAmount;
}

int Process::getProcAmount(){
	return procAmount;
}

int Process::getPid(){
	return pid;
}

void Process::insertSendList(struct eventStruct e){
	sendList.push(e);
}

void Process::handle(){
	Event * cur = getNextEvent();
	if(cur != NULL){
		if(cur->getTime()>=procTime)
			procTime = cur->getTime();
		else
			cout<<"cur->getTime()<procTime "<<cur->toString()<<"\t"<<procTime<<endl;
		cur->handleEvent(blist);
		fout<<cur->toString();
	}
}

void Process::run(){
	int j = 0;
	int i = 0;
	int ret = -1;
	bool fini = false; //current process fini

	initialize();

	while(!fini){
		if(sendList.size()>0)
			sendMessage();
		if(pid != 0 && prevFini != true)
			ret = recvMessage();
		if(prevFini == true && sendList.size() == 0
			&& initQueue.size() == 0 && handQueue.size() == 0){
				struct eventStruct e;
				fini = true;
				e.etype = FINI; e.ano = -1; e.dura = -1; e.bid = -1; e.posInBase = -1;
				e.rc = -1; e.speed = -1; e.time = -1;
				sendList.push(e);
				if(pid != procAmount -1)
					sendMessage();
				break; // no event anymore, execution finished;
		}
		handle();
	}	
	cout<<Event::getResult();
	cout<<pid<<" finish run"<<endl;

}

void Process::sendMessage(){
	MPI_Status stat;
	if(sendFlag == true && sendList.size()>0){
		int dest = -1;
		struct eventStruct e = sendList.top();
		//cout<<e.toString()<<endl;
		if(e.etype == INIT){
			sendList.pop();
			dest = (int)e.bid/baseAmount;
			MPI_Isend(&e, 1, mpiType, dest, 99, MPI_COMM_WORLD, &sendReq);
		} else if((e.etype == FINI && pid+1<procAmount) || (e.etype == HANDO && (e.time<=procTime
			//||prevFini == true
			))){
				sendList.pop();
				dest = pid+1;
				MPI_Isend(&e, 1, mpiType, dest, 99, MPI_COMM_WORLD, &sendReq);
		} else if(e.etype == INITFINI){
			sendList.pop();
			dest = e.bid;
			MPI_Isend(&e, 1, mpiType, dest, 99, MPI_COMM_WORLD, &sendReq);
		} else if(e.etype<0)
			cout<<"sendMessage, etype is "<<e.etype<<endl;
		else
			//cout<<"sendMessage,procTime "<<procTime<<" "<<e.toString()<<endl
			;
	}
	MPI_Test(&sendReq, &sendFlag, &stat);
	while(sendFlag != true){
		handle();
		MPI_Test(&sendReq, &sendFlag, &stat);
	}
}

int Process::recvMessage(){ // rollback should happens here
	MPI_Status stat;
	if(recvFlag == true){
		//cout<<"received ";
		MPI_Irecv(&recvElem, 1, mpiType, MPI_ANY_SOURCE, 99, MPI_COMM_WORLD, &recvReq);
	}
	MPI_Test(&recvReq, &recvFlag, &stat);
	while(recvFlag != true){
		handle();
		MPI_Test(&recvReq, &recvFlag, &stat);
		//cout<<recvElem.toString()<<endl;
	}
	//cout<<recvFlag;
	if(recvFlag == true){
		//cout<<recvElem.toString()<<endl;
		if(recvElem.etype == FINI)
			prevFini = true;
		else if(recvElem.etype == HANDO)
			handQueue.push(new CallHandoverEvent(recvElem));
		else if(recvElem.etype == INIT)
			initQueue.push(new CallInitiationEvent(recvElem));
		else if(recvElem.etype == TERMI)
			cout<<"termi received"<<endl;
		else if(recvElem.etype == INITFINI)
			return INITFINI;
		else
			cout<<"recv something else from "<<stat.MPI_SOURCE<<" "<<recvElem.toString()<<endl;
	}
	return 0;
}

bool Process::queuesClean(){
	return (initQueue.size() == 0 && handQueue.size() == 0);
}
