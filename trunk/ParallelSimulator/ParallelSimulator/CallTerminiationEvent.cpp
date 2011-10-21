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

CallTerminationEvent::CallTerminationEvent(struct eventStruct e)
	:Event(e.time, e.bid, e.ano)
{
	prevCallReserved = e.rc;
}

void CallTerminationEvent::handleEvent(Base blist[]){
	int baseID = getBlistIndex();
	Base * base = &blist[baseID];
	base->decOccupiedChannel();
	if(SCHEME == 1 && prevCallReserved == true)
		base->toggleReservation();
	Event::success++;
	return;
}

void CallTerminationEvent::scheme0(Base blist[]){
	int baseID = getBlistIndex();
	Base * base = &blist[baseID];
	base->decOccupiedChannel();
	Event::success++;
	return;
}

void CallTerminationEvent::scheme1(Base blist[]){
	int baseID = getBlistIndex();
	Base * base = &blist[baseID];
	base->decOccupiedChannel();
	if(prevCallReserved == true){
		base->toggleReservation();
	}
	Event::success++;
	return;
}

string CallTerminationEvent::toString(){
	stringstream ss;
	ss<<"ano:"<<arrivalNo<<"T\ermi\t\t"<<time<<"\t"<<baseId;
	
	//ss<<"t "<<prevCallReserved<<"\t"<<time
		//<<"\t"<<arrivalNo<<"\t";

	//ss<<arrivalNo<<"\t"<<time<<endl;
	return ss.str();
}