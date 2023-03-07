#pragma once
#include "Element.h"
#include <string>
namespace RRS {
	class ButtonSelectable:public Element
	{
	public:
		ButtonSelectable(std::wstring label);
		~ButtonSelectable();
		bool GetIsSelected() { return isSelected; }
		bool SetIsSelected(bool flag) { isSelected = flag; }
	private:
		std::wstring labelStr;
		bool isSelected = false;
	};
}


