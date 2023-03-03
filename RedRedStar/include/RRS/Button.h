#pragma once
#include "Panel.h"
#include "Color.h"
#include <memory>
#include <string>
namespace RRS {
	class Panel;
	class Label;
	class Button : public Panel
	{
	public:
		Button(std::wstring label);
		~Button();
	private:
		std::wstring labelStr;
	};
}


