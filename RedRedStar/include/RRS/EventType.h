#pragma once
namespace RRS {
	class EventListener;
	using EventCallBack = void(EventListener*);
	enum class EventType
	{
		WindowClose,
		WindowClosing,
		Load,
		Hide,
		Show,
		Resize,
		Focs,
		Blur,
	};
}