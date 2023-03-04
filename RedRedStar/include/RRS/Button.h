#pragma once
#include <string>
#include "Element.h"
namespace RRS {
	class Button : public Element
	{
	public:
		Button(std::wstring label);
		~Button();
	private:
		std::wstring labelStr;
	};
}


