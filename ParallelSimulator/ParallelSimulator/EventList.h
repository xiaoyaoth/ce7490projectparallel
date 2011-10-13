#ifndef EVENTLIST_H
#define EVENTLIST_H

#include "Event.h"
#include <queue>

struct comp{
	bool operator() (Event *e1, Event *e2){
		return e1->getTime()>e2->getTime();
	}
};

class EventList
{
private:
	priority_queue<Event*, vector<Event*>, comp> queue;
public:
	EventList(Event * e);
	EventList();
	void insert(Event * e);
	Event * getNextEvent();
};

#endif;