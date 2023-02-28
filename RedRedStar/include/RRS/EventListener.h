#pragma once
#include "EventType.h"
#include <map>
#include <list>
#include <functional>

namespace RRS {
	class EventListener;
	using EventCallBack = std::function<void(EventListener*)>;
	using Dispatcher = std::multimap<EventType, EventCallBack>;
	using EventPointer = void*;
	class EventListener
	{
	public:
		EventListener();
		~EventListener();
		virtual EventPointer AddEventListener(EventType eventType, EventCallBack&& callBack);
		virtual void RemoveEventListener(EventType eventType, EventPointer callBack);
		void EmitEvent(EventType eventType);
	protected:	
		Dispatcher dispatcher;
	};
}