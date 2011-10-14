#include "CallTerminationEvent.h"

CallTerminationEvent::CallTerminationEvent(float t, float pos, int ano)
	:Event(t, pos, ano)
{
	prevCallReserved = false;
}

CallTerminationEvent::CallTerminationEvent(float t, float pos, int ano, bool rc)
	:Event(t, pos, ano)
{
	prevCallReserved = rc;
}

CallTerminationEvent::CallTerminationEvent(eventStruct e)
	:Event(e.time, e.pos, e.ano)
{
	prevCallReserved = e.rc;
}
/*

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
*/

string CallTerminationEvent::toString(){
	stringstream ss;
	int baseID = position/DIAMETER;
	ss<<"ano:"<<arrivalNo<<"Termi\t\t"<<time<<"\t"<<baseID;
	
	//ss<<"t "<<prevCallReserved<<"\t"<<time
		//<<"\t"<<arrivalNo<<"\t";

	//ss<<arrivalNo<<"\t"<<time<<endl;
	return ss.str();
}