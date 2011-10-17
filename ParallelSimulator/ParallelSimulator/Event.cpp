#include "Event.h"
#include "Process.h"

int Event::drop = 0;
int Event::success = 0;
int Event::block = 0;

Event::Event(float t, float pos, int ano){
	time = t;
	position = pos;
	arrivalNo = ano;
	nextEvent = NULL;
}

Event::Event(){
}

float Event::getTime(){
	return time;
}

int Event::getBaseID(){
	return (int)position/DIAMETER;
}

void Event::setNextEventPtr(Event * e){
	nextEvent = e;
}

Event * Event::getNextEventPtr(){
	return nextEvent;
}

int Event::getBlistIndex(){
	return ((int)position/DIAMETER)%Process::getBaseAmount();
}

int Event::getBlistUpperIndex(){
	return Process::getPid()*Process::getBaseAmount();
}

float Event::getDistanceToNextBase(){
	int nextBaseId = 1+(int)position/DIAMETER;
	return nextBaseId*DIAMETER-position;
}

float Event::getNextBasePosition(){
	int nextBaseId = 1+(int)position/DIAMETER;
	return nextBaseId*DIAMETER;
}

void Event::insertIntoEventQueue(Event * e){
	Process::insert(e);
}

void Event::insertIntoSendList(struct eventStruct e){
	Process::insertSendList(e);
}

void Event::handleEvent(Base blist[]){
	std::cout<<"general func of handling event"<<std::endl;
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

int Event::getArrivalNo(){
	return arrivalNo;
}

struct eventStruct Event::toHandoverStruct(int arrivalNo, float duration, float pos, bool prevCallReserved, float speed, float time){
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

struct eventStruct Event::toTerminationStruct(int arrivalNo, float pos, bool prevCallReserved, float time){
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

struct eventStruct Event::toInitiationStruct(int arrivalNo, float duration, float pos, float speed, float time){
        eventStruct e;
        e.ano = arrivalNo;
        e.dura = duration;
        e.etype = 0;
        e.pos = pos;
        e.rc = -1;
        e.speed = speed;
        e.time = time;
        return e;
}