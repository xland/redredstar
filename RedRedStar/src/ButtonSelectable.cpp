#include "../include/RRS/ButtonSelectable.h"
#include "../include/RRS/Label.h"
#include "../include/RRS/Color.h"
namespace RRS {
	ButtonSelectable::ButtonSelectable(std::wstring labelStr)
		: labelStr{ labelStr }
	{
		SetBackgroundColor(GetColor(255, 255, 255));
		SetBackgroundColorHover(GetColor(255, 255, 255));
		SetAlignHorizontal(Align::Center);
		SetAlignVertical(Align::Center);
		auto label = std::make_shared<Label>(labelStr);
		label->SetFontColor(GetColor(0, 0, 0));
		auto w = label->GetWidth() + 50;
		auto h = label->GetHeight() + 30;
		SetWidth(w);
		SetHeight(h);
		AddChild(label);
	}
	ButtonSelectable::~ButtonSelectable()
	{

	}
}