#pragma once
#include "EventType.h"
#include <map>
#include <vector>
#include <functional>

namespace RRS {
	class EventListener;
	using EventCallBack = std::function<void(EventListener*)>;
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