#include "../include/RRS/EventListener.h"
#include "EventDispatcher.h"


namespace RRS {
	
	EventListener::EventListener():eventDispatcher{new EventDispatcher()}
	{
		
	}

	EventListener::~EventListener()
	{
	}
	void EventListener::AddEventListener(EventType eventType, EventCallBack callBack) 
	{
		eventDispatcher->instance.appendListener(eventType, callBack);
	}
	void EventListener::EmitEvent(EventType eventType)
	{
		eventDispatcher->instance.dispatch(eventType, this);
	}
}
