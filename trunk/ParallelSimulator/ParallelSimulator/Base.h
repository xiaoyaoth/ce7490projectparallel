#ifndef BASE_H_
#define BASE_H_

#ifndef BASENO
#define BASENO 20
#endif

#include <string>
#include <sstream>
#include "EventList.h"
#include "Event.h"

using namespace std;//omit this may cause TOSTRING function erronous

class Base{
private:
	int occupiedChannel;
	bool reservedChannelOccupied; // added for the second scheme
	int baseID;
	EventList elist;
	static Base * blist;
public:
	Base();
	Base(int bid);

	int getBaseID();
	void setBaseID(int bid);

	int getOccupiedChannel();
	void incOccupiedChannel();
	void decOccupiedChannel();

	void initializeReservation();
	void toggleReservation();
	bool isReservedChannelOccupied();

	void insert(Event * e);
	Event * getNextEvent();

	string toString();

	static Base* getBlist();
};
#endif
