#include "../include/RRS/ButtonSelectable.h"
#include "../include/RRS/Label.h"
#include "../include/RRS/Color.h"
namespace RRS {
	ButtonSelectable::ButtonSelectable(std::wstring labelStr)
		: labelStr{ labelStr }
	{
		constexpr Color bgColor = GetColor(225, 225, 225);
		constexpr Color bgHoverColor = GetColor(229, 241, 251);
		SetBackgroundColor(bgColor);
		SetBackgroundColorHover(bgHoverColor);
		SetAlignHorizontal(Align::Center);
		SetAlignVertical(Align::Center);
		label = std::make_shared<Label>(labelStr);
		label->SetFontColor(GetColor(255, 255, 255));
		auto w = label->GetWidth() + 50; //todo 可以通过pading来自动设置宽度吗
		auto h = label->GetHeight() + 30;
		SetWidth(w);
		SetHeight(h);
		AddChild(label);
	}
	Color ButtonSelectable::GetCurrentBackgroundColor()
	{
		if (isSelected) 
		{
			return selectedBackgroundColor;
		}
		else
		{
			Color color = GetBackgroundColor();
			if (IsMouseEnter && GetBackgroundColorHover() != UINT32_MAX) {
				color = GetBackgroundColorHover();
			}
			return color;
		}
	}
	void ButtonSelectable::SetSelectedColor(Color color) 
	{
		if (selectedBackgroundColor != color) {
			selectedBackgroundColor = color;
			if (isSelected) {
				SetDirty(true);
			}
		}
	}
	void ButtonSelectable::OnClick()
	{		
		if (isSelected) {
			isSelected = false;
		}
		else {
			isSelected = true;
		}
	}
}