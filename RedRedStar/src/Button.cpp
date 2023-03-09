#include "../include/RRS/Button.h"
#include "../include/RRS/Label.h"
#include "../include/RRS/Color.h"
#include <Windows.h>

namespace RRS {
	Button::Button(std::wstring labelStr)
		: labelStr {labelStr}
	{
		SetBackgroundColor(GetColor(225, 225, 225));
		SetBackgroundColorHover(GetColor(229, 241, 251));
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
	Button::~Button()
	{

	}
}