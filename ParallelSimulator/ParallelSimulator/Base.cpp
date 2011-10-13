#include "Base.h"
#include <iostream>

Base * Base::blist = new Base[BASENO];

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

int Base::getOccupiedChannel(){
	return occupiedChannel;
}

int Base::getBaseID(){
	return baseID;
}

void Base::setBaseID(int bid){
	baseID = bid;
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
	if(10 == baseID)
	ss<<"baseID:"<<baseID<<"\toc:"<<occupiedChannel<<"\trco:"<<reservedChannelOccupied;
	return ss.str();
}

Base * Base::getBlist(){
	return blist;
}