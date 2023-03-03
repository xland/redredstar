#pragma once
namespace RRS {
	struct Point
	{
		float X;
		float Y;
	};
	struct Rectangle
	{
		float X;
		float Y;
		float W;
		float H;
	};
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
		Click,
		/// <summary>
		/// 
		/// </summary>
		MouseOver,
		MouseOut,
	};
}
