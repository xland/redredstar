#include "../include/RRS/Button.h"
#include "../include/RRS/Panel.h"
#include "../include/RRS/Label.h"
#include "../include/RRS/Color.h"
#include "include/core/SkCanvas.h"

namespace RRS {
	Button::Button() 
	{
		SetBackgroundColor(GetColor(88, 28, 156));
		SetBackgroundColorHover(GetColor(28, 88, 156));
		SetFlexDirection(FlexDirection::Column);
		SetJustifyContent(JustifyContent::Center);
		auto label = std::make_shared<Label>("Hello World");
		label->SetAlignSelf(LayoutAlign::Center);
		AddChildElement(label);	
		SetSize(380, 120);
	}
	Button::~Button()
	{

	}
}