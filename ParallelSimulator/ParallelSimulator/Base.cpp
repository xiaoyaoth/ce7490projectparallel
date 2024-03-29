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

string Base::toString(){
	stringstream ss;
	//if(10 == baseID)
	ss<<baseID<<"_"<<occupiedChannel<<"_"<<reservedChannelOccupied;
	return ss.str();
}
