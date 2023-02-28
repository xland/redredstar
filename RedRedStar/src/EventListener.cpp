#include "../include/RRS/EventListener.h"


namespace RRS {
	
	EventListener::EventListener()
	{
		
	}

	EventListener::~EventListener()
	{
	}
	//todo 这些都是同步事件，要搞一套异步事件出来
	EventPointer EventListener::AddEventListener(EventType eventType, EventCallBack&& callBack)
	{
		auto iter = dispatcher.insert({ eventType ,callBack });
		auto a = &iter->second;		
		return a;
	}
	void EventListener::RemoveEventListener(EventType eventType, EventPointer callBack)
	{
		for (auto it = dispatcher.begin(); it != dispatcher.end();)
		{
			if (it->first == eventType && (&it->second) == (EventCallBack*)callBack)
			{
				it = dispatcher.erase(it);
			}
			else
			{
				it++;
			}			
		}
	}
	void EventListener::EmitEvent(EventType eventType)
	{
		auto range = dispatcher.equal_range(eventType);
		for (auto it = range.first; it != range.second; ++it) {
			it->second(this);
		}
	}
}
