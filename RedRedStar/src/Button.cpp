#include "../include/RRS/Button.h"
#include "../include/RRS/Panel.h"
#include "../include/RRS/Label.h"
#include "../include/RRS/Color.h"
#include "../include/RRS/Window.h"
#include "include/core/SkCanvas.h"
#include <Windows.h>

namespace RRS {
	Button::Button() 
	{
		SetBackgroundColor(GetColor(88, 28, 156));
		SetBackgroundColorHover(GetColor(28, 88, 156));
		SetFlexDirection(FlexDirection::Column);
		SetJustifyContent(JustifyContent::Center);
		this->SetBorderRadius(4.f);
		auto label = std::make_shared<Label>(L"Hello ÊÀ½ç");
		label->SetFontColor(GetColor(255, 255, 255));
		label->SetAlignSelf(LayoutAlign::Center);
		AddChildElement(label);	
		SetSize(label->GetWidth() + 50, 60);

		
		AddEventListener(EventType::Click, [this](EventListener* arg) {
			InvalidateRect(this->GetOwnerWindow()->Hwnd, nullptr, false);
		});
	}
	Button::~Button()
	{

	}
}