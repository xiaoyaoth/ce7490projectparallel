#include "Event.h"
#include "Process.h"
#include <iostream>

int Event::drop = 0;
int Event::success = 0;
int Event::block = 0;

Event::Event(float t, int bid, int ano){
	time = t;
	baseId = bid;
	arrivalNo = ano;
}

Event::Event(){
}

/*Logic*/
float Event::getTime(){
	return time;
}

int Event::getBaseID(){
	return baseId;
}

void Event::handleEvent(Base blist[]){
	std::cout<<"general func of handling event"<<std::endl;
}

void Event::redo(Base blist[]){
	std::cout<<"general func of reverse Execution"<<std::endl;
}

string Event::toString(){
	stringstream ss;
	ss<<"vitual"<<"\t"<<time<<"\t"<<std::endl;
	return ss.str();
}

string Event::getResult(){
	stringstream ss;
	ss<<"success\tdrop\tblock\t"<<endl;
	ss<<Event::success<<"\t"<<Event::drop<<"\t"<<Event::block<<endl;
	return ss.str();
}

/*MPI*/
void Event::insertIntoEventQueue(Event * e){
	Process::insert(e);
}

void Event::insertIntoSendList(struct eventStruct e){
	Process::insertSendList(e);
}

int Event::getBlistIndex(){
	return baseId%Process::getBaseAmount();
}

int Event::getBlistSize(){
	return Process::getBaseAmount();
}

int Event::getArrivalNo(){
	return arrivalNo;
}

struct eventStruct Event::toInitiationStruct(int arrivalNo, float duration, float pos, float speed, float time){
        eventStruct e;
        e.ano = arrivalNo;
        e.dura = duration;
        e.etype = 0;
		e.bid = (int)pos/DIAMETER;
        e.posInBase = pos-DIAMETER*e.bid;
        e.rc = -1;
        e.speed = speed;
        e.time = time;
        return e;
}

struct eventStruct Event::toHandoverStruct(int arrivalNo, float duration, int bid, bool prevCallReserved, float speed, float time){
        eventStruct e;
        e.ano = arrivalNo;
        e.dura = duration;
        e.etype = 1;
		e.bid = bid;
        e.posInBase = 0;
        e.rc = (int)prevCallReserved;
        e.speed = speed;
        e.time = time;
        return e;
}

struct eventStruct Event::toTerminationStruct(int arrivalNo, int bid, bool prevCallReserved, float time){
        eventStruct e;
        e.ano = arrivalNo;
        e.dura = -1;
        e.etype = 2;
		e.bid = bid;
        e.posInBase = -1;
        e.rc = prevCallReserved;
        e.speed = -1;
        e.time = time;
        return e;
}
