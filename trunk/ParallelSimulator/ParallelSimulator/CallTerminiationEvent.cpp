#include "CallTerminationEvent.h"

CallTerminationEvent::CallTerminationEvent(float t, int bid, int ano)
	:Event(t, bid, ano)
{
	prevCallReserved = false;
}

CallTerminationEvent::CallTerminationEvent(float t, int bid, int ano, bool rc)
	:Event(t, bid, ano)
{
	prevCallReserved = rc;
}

void CallTerminationEvent::handleEvent(Base blist[]){
	if(SCHEME == 0)
		scheme0(blist);
	else
		scheme1(blist);
}

void CallTerminationEvent::scheme0(Base blist[]){
	Base * base = &blist[baseID];
	base->decOccupiedChannel();
	Event::success++;
	return;
}

void CallTerminationEvent::scheme1(Base blist[]){
	Base * base = &blist[baseID];
	base->decOccupiedChannel();
	if(prevCallReserved == true){
		base->toggleReservation();
	}
	Event::success++;
	return;
}

string CallTerminationEvent::getOutput(Base blist[]){
	stringstream ss;
	//ss<<this->getEventID()<<"\t"<<"Termi\t\t"<<time<<"\t"<<baseID<<std::endl;

	ss<<"t "<<prevCallReserved<<"\t"<<time
		<<"\t"<<arrivalNo<<"\t"<<blist[baseID].toString()
		<<endl;

	//ss<<arrivalNo<<"\t"<<time<<endl;
	return ss.str();
}