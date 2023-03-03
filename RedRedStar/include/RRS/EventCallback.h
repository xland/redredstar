#pragma once
#include <functional>
#include "../include/RRS/CommonType.h"

namespace RRS {
	class EventListener;
	class EventCallback
	{
	public:
		EventCallback(std::function<void(EventListener*)>&& callBack);
		~EventCallback();
		void Execute(EventListener* target);
		int Id;
	private:
		std::function<void(EventListener*)> callBack;
	};
}


