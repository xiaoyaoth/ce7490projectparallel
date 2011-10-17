#include "CallHandoverEvent.h"
#include "CallTerminationEvent.h"

CallHandoverEvent::CallHandoverEvent(float t, float s, float pos, float d, int ano)
	:Event(t, pos, ano)
{
	speed = s;
	duration = d;
	prevCallReserved = false;
}


CallHandoverEvent::CallHandoverEvent(float t, float s, float pos, float d, int ano, bool rc)
	:Event(t, pos, ano)
{
	speed = s;
	duration = d;
	prevCallReserved = rc;
}

CallHandoverEvent::CallHandoverEvent(struct eventStruct e)
	:Event(e.time, e.pos, e.ano)
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
	int baseID = getBlistIndex();
	Base *base = &blist[baseID];
	Base *prevBase = &blist[baseID-1];
	prevBase->decOccupiedChannel();
	int oc = base->getOccupiedChannel(); //occupied channel amount
	if(oc<10){//not all the channel occupied
		base->incOccupiedChannel();
		float handoverTS = time+3600*DIAMETER/speed;
		float terminationTS = time + duration;
		if(handoverTS<terminationTS)
			if(baseID+1<20)
				new CallHandoverEvent(handoverTS, speed, baseID+1, terminationTS-handoverTS, arrivalNo);
			else
				new CallTerminationEvent(handoverTS, baseID, arrivalNo);
		else
			new CallTerminationEvent(terminationTS, baseID, arrivalNo);
	}else //all the channel occupied
		Event::drop++;
	return;
}

void CallHandoverEvent::scheme1(Base * blist){
	int baseID = getBlistIndex();
	Base *base = &blist[baseID];
	Base *prevBase = &blist[baseID-1];
	prevBase->decOccupiedChannel();
	if(prevCallReserved == true)
		prevBase->toggleReservation(); //?

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
		if(handoverTS<terminationTS)
			if(baseID+1<20)
				new CallHandoverEvent(handoverTS, speed, baseID+1, terminationTS-handoverTS, arrivalNo, rc);
			else
				new CallTerminationEvent(handoverTS, baseID, arrivalNo, rc);
		else
			new CallTerminationEvent(terminationTS, baseID, arrivalNo, rc);
	}else //all the channel occupied
		Event::drop++;
	return;
}

string CallHandoverEvent::toString(){
	stringstream ss;
	int baseID = (int)position/DIAMETER;

	ss<<"ano:"<<arrivalNo<<"Hando\t\t"<<time
		<<"\t"<<baseID<<"\t"<<speed<<"\t"<<duration;
	
	//ss<<"h "<<this->prevCallReserved<<"\t"<<time
		//<<"\t"<<arrivalNo<<"\t";

	//ss<<arrivalNo<<"\t"<<time<<endl;
	return ss.str();
}
