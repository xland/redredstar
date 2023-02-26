#pragma once
#include "EventType.h"
#include <map>
#include <vector>

namespace RRS {
	using Dispatcher = std::map<EventType, std::vector<EventCallBack>>;
	class EventListener
	{
	public:
		EventListener();
		~EventListener();
		void AddEventListener(EventType eventType, EventCallBack callBack);
		void EmitEvent(EventType eventType);
	protected:	
		Dispatcher dispatcher;
	};
}