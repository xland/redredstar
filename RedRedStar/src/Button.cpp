#include "../include/RRS/Button.h"
#include "../include/RRS/Panel.h"
#include "../include/RRS/Label.h"
#include "../include/RRS/Color.h"
#include "../include/RRS/Window.h"
#include "include/core/SkCanvas.h"
#include <Windows.h>

namespace RRS {
	Button::Button(std::wstring labelStr): labelStr {labelStr}
	{
		
		SetBackgroundColor(GetColor(88, 28, 156));
		SetBackgroundColorHover(GetColor(28, 88, 156));
		SetFlexDirection(FlexDirection::Column);
		SetJustifyContent(JustifyContent::Center);
		this->SetBorderRadius(4.f);
		auto label = std::make_shared<Label>(labelStr);
		label->SetFontColor(GetColor(255, 255, 255));
		label->SetAlignSelf(LayoutAlign::Center);
		AddChildElement(label);	
		SetSize(label->GetWidth() + 50, 60);		
	}
	Button::~Button()
	{

	}
}