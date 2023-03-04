#pragma once
#include "CommonType.h"
#include <map>
#include <list>
#include <memory>
#include <functional>

namespace RRS {
	class EventCallback;
	class EventListener
	{
	public:
		EventListener();
		~EventListener();
		virtual int AddEventListener(EventType eventType, std::function<void()>&& callBack);
		virtual void RemoveEventListener(int callBackId);
		void EmitEvent(EventType eventType);
	protected:	
		std::multimap<EventType, std::shared_ptr<EventCallback>> dispatcher;
	};
}