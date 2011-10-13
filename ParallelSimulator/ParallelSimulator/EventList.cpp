#include "EventList.h"
#include <iostream>
#include <queue>

EventList::EventList(Event *e){
	queue.empty();
}

void EventList::insert(Event * e){
	queue.push(e);
	return;
}

Event * EventList::getNextEvent(){
	if(queue.size()>0){
		Event * ret = queue.top();
		queue.pop();
		return ret;
	} else
		return NULL;
}
