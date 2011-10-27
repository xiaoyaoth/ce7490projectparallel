#ifndef BASE_H_
#define BASE_H_

#ifndef BASENO
#define BASENO 20
#endif

#include <string>
#include <sstream>
#include <list>

using namespace std;//omit this may cause TOSTRING function erronous

struct baseState{
	float time;
	bool rco;
	int oc;
	
	string toString(){
		stringstream ss;
		ss<<time<<"_"<<oc<<"_"<<rco<<" || ";
		return ss.str();
	}
};

class Base{
private:
	int occupiedChannel;
	bool reservedChannelOccupied; // added for the second scheme
	int baseID;
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

	void saveState(float t);
	
	string printStateList();
	string toString();
};
#endif

