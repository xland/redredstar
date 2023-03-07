#pragma once
#include "Element.h"
#include <string>
namespace RRS {
	class ButtonSelectable:public Element
	{
	public:
		ButtonSelectable(std::wstring label);
		~ButtonSelectable();
	private:
		std::wstring labelStr;

	};
}


