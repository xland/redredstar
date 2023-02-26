#pragma once
#include "../include/RRS/EventType.h"
#include "event/eventdispatcher.h"

namespace RRS {
	using InternalEventDispatcher = eventpp::EventDispatcher<EventType, EventCallBack>;
	class EventDispatcher
	{
		public:
			EventDispatcher();
			~EventDispatcher();
			InternalEventDispatcher instance;
		private:
			
	
	};
}

