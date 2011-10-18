#include "Process.h"
#include "CallHandoverEvent.h"
#include "CallInitiationEvent.h"
#include "CallTerminationEvent.h"
#include <string>

int Process::baseAmount = 0;
int Process::procAmount = 0;
int Process::pid = 0;
priority_queue<Event*, vector<Event*>, comp> Process::queue;
list<struct eventStruct> Process::sendList;

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
	queue.empty();
}

void Process::insert(Event * e){
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
	int ret = 0; //received event type
	string rec; //one record
	struct eventStruct e;
	ifstream fin;

	stringstream ss;
	ss<<pid<<".txt";
	ofstream fout(ss.str().c_str());

	int i = 0;
	bool fini = false; //current process fini
	bool prevFini = false; // previous process has finished;

	if(pid == 0){
		fin.open("C:\\Users\\xli15\\Documents\\Visual Studio 2010\\Projects\\ParallelSimulator\\ParallelSimulator\\data.txt");
		if(!fin)
			cout<<"file not exist"<<endl;
	}

	while(!fini){
		i++;
		if(pid == 0 && !fin.eof()){
			getline(fin, rec);
			e =  parseData(rec);
			//cout<<e.ano<<" "<<e.getBaseID()/baseAmount<<" "
				//<<e.getBaseID()<<" "<<baseAmount<<endl;
			if(e.bid<baseAmount){
				this->insert(new CallInitiationEvent(e));
			}
			else{
				sendList.push_back(e);
			}
		} else
			ret = FINI;
		sendMessage();
		if(pid != 0)
			ret = recvMessage();
		if(ret == FINI){
			prevFini = true;
		}
		if(prevFini == true && queue.size() == 0){
			fini = true;
			e.etype = FINI;
			sendList.push_back(e);
			if(pid != procAmount -1)
				sendMessage();
			break; // no event anymore, execution finished;
		}
		Event * cur = getNextEvent();
		if(cur != NULL){
			fout<<cur->toString()<<"\t";
			fout<<blist[cur->getBlistIndex()].toString()<<endl;
			cur->handleEvent(blist);
		}
	}
	cout<<"finish run, while loop: "<<i<<endl;
}

void Process::sendMessage(){
	MPI_Status stat;
	if(sendFlag == true && sendList.size()>0){
		struct eventStruct e = sendList.front();
		sendList.pop_front();
		int dest = -1;
		if(e.etype == INIT)
			dest = (int)e.bid/baseAmount;
		else if(e.etype == DECPREV)
			dest = pid-1;
		else
			dest = pid+1;
		if(dest>=procAmount)
			e.toString();
		MPI_Isend(&e, 1, mpiType, dest, 99, MPI_COMM_WORLD, &sendReq);
	}
	MPI_Test(&sendReq, &sendFlag, &stat);
}

int Process::recvMessage(){ // rollback should happens here
	MPI_Status stat;
	if(recvFlag == true){
		//cout<<"received ";
		MPI_Irecv(&recvElem, 1, mpiType, MPI_ANY_SOURCE, 99, MPI_COMM_WORLD, &recvReq);
	}
	MPI_Test(&recvReq, &recvFlag, &stat);
	if(recvFlag == true){
		//recvElem.toString();
		if(recvElem.etype == FINI)
			return FINI;
		else if(recvElem.etype == HANDO)
			this->insert(new CallHandoverEvent(recvElem));
		else if(recvElem.etype == INIT)
			this->insert(new CallInitiationEvent(recvElem));
		else if(recvElem.etype == TERMI)
			this->insert(new CallTerminationEvent(recvElem));
		else if(recvElem.etype == DECPREV){
			int bidx = recvElem.bid%baseAmount;
			if(bidx!=baseAmount-1)
				cout<<"bidx!=baseAmount-1"<<endl;
			else
				blist[bidx].decOccupiedChannel();
		}
		return 0;
	}
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
	if(e.etype == DECPREV){
		sendList.push_front(e);
		sendList.front().toString();
	}
	else
		sendList.push_back(e);
}