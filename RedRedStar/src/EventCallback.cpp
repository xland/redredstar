#include "../include/RRS/EventCallback.h"
#include <random>
namespace RRS {
	static int index = 0;
	EventCallback::EventCallback(std::function<void(EventListener*)>&& callBack)
		:callBack{ callBack },Id{index++}
	{
		
	}
	EventCallback::~EventCallback()
	{
	}
	void EventCallback::Execute(EventListener* target)
	{
		callBack(target);
	}
}
