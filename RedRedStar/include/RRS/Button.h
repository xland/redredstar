#pragma once
#include <string>
#include "Element.h"
namespace RRS {
	class Label;
	class Button : public Element
	{
	public:
		Button(std::wstring text);
		~Button();
		std::shared_ptr<Label> label;
	private:
		std::wstring labelStr;
	};
}


