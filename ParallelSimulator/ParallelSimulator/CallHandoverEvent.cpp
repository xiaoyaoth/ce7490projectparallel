#include "CallHandoverEvent.h"
#include "CallTerminationEvent.h"

CallHandoverEvent::CallHandoverEvent(float t, float s, int bid, float d, int ano)
	:Event(t, bid, ano)
{
	speed = s;
	duration = d;
	prevCallReserved = false;
}

CallHandoverEvent::CallHandoverEvent(float t, float s, int bid, float d, int ano, bool rc)
	:Event(t, bid, ano)
{
	speed = s;
	duration = d;
	prevCallReserved = rc;
}

CallHandoverEvent::CallHandoverEvent(struct eventStruct e)
	:Event(e.time, e.bid, e.ano)
{
	speed = e.speed;
	duration = e.dura;
	prevCallReserved = (bool)e.rc;
}

void CallHandoverEvent::handleEvent(Base blist[]){
	if(SCHEME == 0)
		scheme0(blist);
	else
		scheme1(blist);
}

void CallHandoverEvent::scheme0(Base * blist){
	int rc = 0;//prevCallReserved is 0 in scheme 0;
	int bidx = getBlistIndex();
	Base *base = &blist[bidx];
	int oc = base->getOccupiedChannel(); //occupied channel amount
	if(oc<10){//not all the channel occupied
		base->incOccupiedChannel();
		float handoverTS = time+3600*DIAMETER/speed;
		float terminationTS = time + duration;
		if(handoverTS<terminationTS){
			if(baseId+1<20){
				if(bidx+1<getBlistSize()){
					CallHandoverEvent * ev = new CallHandoverEvent(handoverTS, 
						speed, baseId+1, terminationTS-handoverTS, arrivalNo);
					insertIntoEventQueue(ev);
				} else {
					struct eventStruct es = toHandoverStruct(arrivalNo, 
						terminationTS-handoverTS, baseId+1, rc, speed, handoverTS);
					insertIntoSendList(es);
				}
			}
			CallTerminationEvent *cte = new CallTerminationEvent(handoverTS, baseId, arrivalNo);
			cte->print = false;
			insertIntoEventQueue(cte);
		} else
			insertIntoEventQueue(new CallTerminationEvent(terminationTS, baseId, arrivalNo));
	}else //all the channel occupied
		Event::drop++;
	return;
}

void CallHandoverEvent::scheme1(Base * blist){
	int bidx = getBlistIndex();
	Base *base = &blist[bidx];

	int oc = base->getOccupiedChannel();
	bool rco = base->isReservedChannelOccupied();//reservedChannleOccupied
	bool rc = false; // define whether this newly-generated handover here is a reserved call or not;
	if(oc<10){//not all the channel occupied
		if(oc == 9 && rco == false){
			base->toggleReservation();
			rc = true;
		}
		base->incOccupiedChannel();
		float handoverTS = time+3600*DIAMETER/speed;
		float terminationTS = time + duration;
		if(handoverTS<terminationTS){
			if(baseId+1<20){
				if(bidx+1<getBlistSize()){
					CallHandoverEvent * ev = new CallHandoverEvent(handoverTS, 
						speed, baseId+1, terminationTS-handoverTS, arrivalNo, rc);
					insertIntoEventQueue(ev);
				} else {
					struct eventStruct es = toHandoverStruct(arrivalNo, 
						terminationTS-handoverTS, baseId+1, rc, speed, handoverTS);
					insertIntoSendList(es);
				}
			}
			CallTerminationEvent *cte = new CallTerminationEvent(handoverTS, baseId, arrivalNo, rc);
			cte->print = false;
			insertIntoEventQueue(cte);
		}else
			insertIntoEventQueue(new CallTerminationEvent(terminationTS, baseId, arrivalNo, rc));
	}else //all the channel occupied
		Event::drop++;
	return;
}

string CallHandoverEvent::toString(){
	stringstream ss;

	//ss<<"ano:"<<arrivalNo<<"\tHando\t\t"<<time
	//	<<"\t"<<baseId<<"\t"<<speed<<"\t"<<duration;

	ss<<"h "<<this->prevCallReserved<<"\t"<<time
		<<"\t"<<arrivalNo;

	//ss<<arrivalNo<<"\t"<<time<<endl;
	return ss.str();
}
