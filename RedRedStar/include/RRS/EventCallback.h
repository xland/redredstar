#pragma once
#include <functional>

namespace RRS {
	using CallbackId = unsigned int;
	class EventListener;
	class EventCallback
	{
	public:
		EventCallback(std::function<void()>&& callBack);
		~EventCallback();
		void Execute();
		CallbackId Id;
	private:
		std::function<void()> callBack;
	};
}


