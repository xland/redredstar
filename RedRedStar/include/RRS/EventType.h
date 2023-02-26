#pragma once
#include <functional>
namespace RRS {	
	class EventListener;
	using EventCallBack = std::function<void(EventListener*)>;
	enum class EventType
	{
		WindowClosed,
		WindowClosing,
		Loaded,
		Hide,
		Show,
		Resize,
		Focs,
		Blur,
	};
}