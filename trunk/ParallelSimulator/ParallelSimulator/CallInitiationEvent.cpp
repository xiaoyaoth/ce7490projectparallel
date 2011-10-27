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
	int bidx = getBlistIndex();
	Base *base = &blist[bidx];
	int oc = base->getOccupiedChannel(); //occupied channel 
	//cout<<base->toString();
	if(oc<10){
		base->incOccupiedChannel();
		float handoverTS = time + 3600*(DIAMETER-posInBase)/speed;
		float terminationTS = time + duration;
		if(handoverTS<terminationTS){
			if(baseId+1<20)
				if(bidx+1<getBlistSize()){
					CallHandoverEvent *ev = new CallHandoverEvent(handoverTS, 
						speed, baseId+1, terminationTS-handoverTS, arrivalNo);
					insertIntoEventQueue(ev);
				}
				else
					insertIntoSendList(toHandoverStruct(arrivalNo, terminationTS-handoverTS,
					baseId+1, rc, speed, handoverTS));
			CallTerminationEvent *cte = new CallTerminationEvent(handoverTS, baseId, arrivalNo);
			cte->print = false;
			insertIntoEventQueue(cte);
		} else
			insertIntoEventQueue(new CallTerminationEvent(terminationTS, baseId, arrivalNo));
	}else
		Event::block++;
	return;
}

void CallInitiationEvent::scheme1(Base blist[]){
	int bidx = getBlistIndex();
	Base *base = &blist[bidx];
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
		if(handoverTS<terminationTS){
			if(baseId+1<20)
				if(bidx+1<getBlistSize()){
					CallHandoverEvent *ev = new CallHandoverEvent(handoverTS, 
						speed, baseId+1, terminationTS-handoverTS, arrivalNo);
					insertIntoEventQueue(ev);
				} else
					insertIntoSendList(toHandoverStruct(arrivalNo, terminationTS-handoverTS,
					baseId+1, 0, speed, handoverTS));
			CallTerminationEvent *cte = new CallTerminationEvent(handoverTS, baseId, arrivalNo);
			cte->print = false;
			insertIntoEventQueue(cte);
		}else
			insertIntoEventQueue(new CallTerminationEvent(terminationTS, baseId, arrivalNo, false));
	}else
		Event::block++;
	return;
}
