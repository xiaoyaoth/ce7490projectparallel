#include "CallInitiationEvent.h"
#include "CallHandoverEvent.h"
#include "CallTerminationEvent.h"

CallInitiationEvent::CallInitiationEvent(float t, float s, float p, float d, int no)
	:Event(t, (int)p/DIAMETER, no)
{
	posInBase = p-DIAMETER*((int)p/DIAMETER);
	speed = s;
	duration = d;
}

CallInitiationEvent::CallInitiationEvent(struct eventStruct e)
	:Event(e.time, e.bid, e.ano)
{
	posInBase = e.posInBase;
	speed = e.speed;
	duration = e.dura;
}

void CallInitiationEvent::handleEvent(Base blist[]){
	if(SCHEME == 0)
		scheme0(blist);
	else
		scheme1(blist);
}

void CallInitiationEvent::scheme0(Base blist[]){
	int rc = 0;//prevCallReserved is 0 in scheme 0;
	int bid = getBaseID();
	int bidx = getBlistIndex();
	Base *base = &blist[bidx];
	int oc = base->getOccupiedChannel(); //occupied channel 
	//cout<<base->toString();
	if(oc<10){
		base->incOccupiedChannel();
		float handoverTS = time + 3600*(DIAMETER-posInBase)/speed;
		float terminationTS = time + duration;
		if(handoverTS<terminationTS)
			if(bid+1<20)
				if(bidx+1<getBlistSize())
					insertIntoEventQueue(
					new CallHandoverEvent(handoverTS, speed, bid+1, 
					terminationTS-handoverTS, arrivalNo)
					);
				else
					insertIntoSendList(toHandoverStruct(arrivalNo, terminationTS-handoverTS,
					bid+1, rc, speed, handoverTS));
			else
				new CallTerminationEvent(handoverTS, bid, arrivalNo);
		else
			new CallTerminationEvent(terminationTS, bid, arrivalNo);
	}else
		Event::block++;
	return;
}

void CallInitiationEvent::scheme1(Base blist[]){
	int baseID = getBlistIndex();
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
		float handoverTS = time + 3600*(DIAMETER-posInBase)/speed;
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

string CallInitiationEvent::toString(){
	stringstream ss;

	ss<<"ano:"<<arrivalNo<<"\tInit\t"<<arrivalNo<<"\t"<<time<<"\t"
		<<baseId<<"\t"<<speed<<"\t"<<duration<<"\t"<<posInBase;

	//ss<<"i"<<"\t"<<time
	//<<"\t"<<arrivalNo<<"\t";

	//ss<<arrivalNo<<"\t"<<time<<endl;
	return ss.str();
}