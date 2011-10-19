#include "Base.h"

Base::Base(){
	baseID = 0;
	occupiedChannel = 0;
	reservedChannelOccupied = false;
}

Base::Base(int bid){
	occupiedChannel = 0;
	baseID = bid;
	reservedChannelOccupied = false;
}

int Base::getBaseID(){
	return baseID;
}

void Base::setBaseID(int bid){
	baseID = bid;
}


int Base::getOccupiedChannel(){
	return occupiedChannel;
}

void Base::incOccupiedChannel(){
	occupiedChannel++;
}

void Base::decOccupiedChannel(){
	occupiedChannel--;
}

void Base::initializeReservation(){
	reservedChannelOccupied = false;
	return;
}

void Base::toggleReservation(){
	//if(baseID == 10)
	//	cout<<baseID<<" "<<reservedChannelOccupied<<" "<<occupiedChannel<<endl;
	if(reservedChannelOccupied == true)
		reservedChannelOccupied = false;
	else
		reservedChannelOccupied = true;
	return;
}

bool Base::isReservedChannelOccupied(){
	return reservedChannelOccupied;
}

void Base::saveState(float time){
	struct baseState bs;
	bs.time = time;
	bs.rco = reservedChannelOccupied;
	bs.oc = occupiedChannel;
	stateList.push_back(bs);
}

string Base::printStateList(){
	stringstream ss;
	ss<<"bid:"<<baseID<<endl;
	while(stateList.size()>0){
		ss<<stateList.front().toString();
		stateList.pop_front();
	}
	return ss.str();
}

string Base::toString(){
	stringstream ss;
	//if(10 == baseID)
	ss<<baseID<<"_"<<occupiedChannel<<"_"<<reservedChannelOccupied<<" || ";
	return ss.str();
}
