#include "../include/RRS/Button.h"
#include "../include/RRS/Label.h"
#include "../include/RRS/Color.h"
#include <Windows.h>

namespace RRS {
	Button::Button(std::wstring labelStr)
		: labelStr {labelStr}
	{
		SetBackgroundColor(GetColor(88, 28, 156));
		SetBackgroundColorHover(GetColor(28, 88, 156));
		SetAlignHorizontal(Align::Center);
		SetAlignVertical(Align::Center);
		auto label = std::make_shared<Label>(labelStr);
		label->SetFontColor(GetColor(255, 255, 255));
		auto w = label->GetWidth() + 50;
		auto h = label->GetHeight() + 30;
		SetWidth(w);
		SetHeight(h);
		AddChild(label);
	}
	Button::~Button()
	{

	}
}