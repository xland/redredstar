#pragma once
#include "EventType.h"

namespace RRS {
	class EventDispatcher;
	class EventListener
	{
	public:
		EventListener();
		~EventListener();
		void AddEventListener(EventType eventType, EventCallBack callBack);
		void EmitEvent(EventType eventType);
	private:
		EventDispatcher* eventDispatcher;
	};
}