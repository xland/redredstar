#pragma once
namespace RRS {
	enum class Align
	{
		Start,
		Center,
		End,
		/// <summary>
		/// |box|space|box|space|box|
		/// </summary>
		SpaceBetween,
		/// <summary>
		/// |half-space|box|space|box|space|box|half-space|
		/// </summary>
		SpaceAround,
		/// <summary>
		/// |space|box|space|box|space|box|space|
		/// </summary>
		SpaceEvenly,
		Flex,
	};
	//todo AlignSelf
	enum class LayoutDirection
	{
		Column,
		Row
	};
}