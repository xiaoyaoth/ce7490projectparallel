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
	return (int)position/2;
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

/* http://en.wikipedia.org/wiki/Virtual_function 
what does the const mean in the C++ description of virtual function*/

