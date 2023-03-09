#pragma once
#include "Button.h"
#include <string>
namespace RRS {
	class Label;
	class ButtonSelectable:public Element
	{
	public:
		ButtonSelectable(std::wstring label);
		void SetSelectedColor(Color color);
		Color GetSelectedColor() { return selectedBackgroundColor; };  //todo
		bool GetIsSelected() { return isSelected; }
		bool SetIsSelected(bool flag) { isSelected = flag; }
		Color GetCurrentBackgroundColor() override;
		void OnClick() override;
		std::shared_ptr<Label> label;
	private:
		std::wstring labelStr;
		bool isSelected{ false };
		Color selectedBackgroundColor{ GetColor(205,232,255)};
	};
}


