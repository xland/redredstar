#include "../include/RRS/EventListener.h"


namespace RRS {
	
	EventListener::EventListener()
	{
		
	}

	EventListener::~EventListener()
	{
	}
	//todo 这些都是同步事件，要搞一套异步事件出来
	void EventListener::AddEventListener(EventType eventType, EventCallBack callBack) 
	{
		dispatcher[eventType].push_back(callBack);
	}
	void EventListener::EmitEvent(EventType eventType)
	{
		for (auto& cb : dispatcher[eventType])
		{
			cb(this);
		}
	}
}
