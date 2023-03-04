#include "../include/RRS/EventCallback.h"
#include <random>
namespace RRS {
	static unsigned int index = 0;
	EventCallback::EventCallback(std::function<void()>&& callBack)
		:callBack{ callBack },Id{index++}
	{
		
	}
	EventCallback::~EventCallback()
	{
	}
	void EventCallback::Execute()
	{
		callBack();
	}
}
