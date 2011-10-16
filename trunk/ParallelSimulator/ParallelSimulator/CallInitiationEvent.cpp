#include "CallInitiationEvent.h"

CallInitiationEvent::CallInitiationEvent(float t, float s, float p, float d, int no)
	:Event(t, p, no)
{
	speed = s;
	duration = d;
}

CallInitiationEvent::CallInitiationEvent(eventStruct e)
	:Event(e.time, e.pos, e.ano)
{
	speed = e.speed;
	duration = e.dura;
}
/*
void CallInitiationEvent::handleEvent(Base blist[]){
	if(SCHEME == 0)
		scheme0(blist);
	else
		scheme1(blist);
}

void CallInitiationEvent::scheme0(Base blist[]){
	Base *base = &blist[baseID];
	int oc = base->getOccupiedChannel(); //occupied channel 
	//cout<<base->toString();
	if(oc<10){
		base->incOccupiedChannel();
		float handoverTS = time + 3600*(DIAMETER-position)/speed;
		float terminationTS = time + duration;
		if(handoverTS<terminationTS)
			if(baseID+1<20)
				new CallHandoverEvent(handoverTS, speed, baseID+1, terminationTS-handoverTS, arrivalNo);
			else
				new CallTerminationEvent(handoverTS, baseID, arrivalNo);
		else
			new CallTerminationEvent(terminationTS, baseID, arrivalNo);
	}else
		Event::block++;
	return;
}

void CallInitiationEvent::scheme1(Base blist[]){
	Base * base = &blist[baseID];
	int oc = base->getOccupiedChannel(); //occupied channel amount
	bool rco = base->isReservedChannelOccupied();//reservedChannleOccupied
	int avc; //availabe channel amount
	if(rco)
		avc = 10-oc;
	else
		avc = 9-oc;
	if(avc>0){
		base->incOccupiedChannel();
		float handoverTS = time + 3600*(DIAMETER-position)/speed;
		float terminationTS = time + duration;
		if(handoverTS<terminationTS)
			if(baseID+1<20)
				new CallHandoverEvent(handoverTS, speed, baseID+1, terminationTS-handoverTS, arrivalNo, false);
			else
				new CallTerminationEvent(handoverTS, baseID, arrivalNo, false);
		else
			new CallTerminationEvent(terminationTS, baseID, arrivalNo, false);
	}else
		Event::block++;
	return;
}
*/
string CallInitiationEvent::toString(){
	stringstream ss;
	int baseID = (int)position/DIAMETER;

	ss<<"ano:"<<arrivalNo<<"\tInit\t"<<arrivalNo<<"\t"<<time<<"\t"
		<<baseID<<"\t"<<speed<<"\t"<<duration<<"\t"<<position;
	
	//ss<<"i"<<"\t"<<time
		//<<"\t"<<arrivalNo<<"\t";

	//ss<<arrivalNo<<"\t"<<time<<endl;
	return ss.str();
}