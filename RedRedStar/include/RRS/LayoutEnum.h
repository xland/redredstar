#pragma once
namespace RRS {
	enum class Align
	{
		Start,
		Center,
		End,
		SpaceBetween, // |- - -|
		SpaceAround // | - - - |
	};
	enum class FlexDirection
	{
		Column,
		Row
	};
}