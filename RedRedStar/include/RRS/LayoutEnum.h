#pragma once
namespace RRS {
	enum class Align
	{
		Start,
		Center,
		End,
		SpaceBetween, // |- - -|
		SpaceAround, // | - - - |
		Flex,
	};
	//todo AlignSelf
	enum class LayoutDirection
	{
		Column,
		Row
	};
}