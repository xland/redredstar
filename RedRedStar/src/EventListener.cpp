#include "../include/RRS/EventListener.h"
#include "../include/RRS/EventCallback.h"

namespace RRS {
	
	EventListener::EventListener()
	{
		
	}

	EventListener::~EventListener()
	{
	}
	//todo 这些都是同步事件，要搞一套异步事件出来
	int EventListener::AddEventListener(EventType eventType, std::function<void(EventListener*)> callBack)
	{
		std::shared_ptr<EventCallback> functor = std::make_shared<EventCallback>(callBack);
		dispatcher.insert({ eventType ,functor });
		return functor->Id;
	}
	void EventListener::RemoveEventListener(EventType eventType, int callBackId)
	{
		for (auto it = dispatcher.begin(); it != dispatcher.end();)
		{
			if (it->first == eventType && it->second->Id == callBackId)
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
			it->second->Execute(this);
		}
	}
}
