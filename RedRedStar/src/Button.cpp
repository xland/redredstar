#include "../include/RRS/Button.h"
#include "../include/RRS/Label.h"
#include "../include/RRS/Color.h"
#include "include/core/SkCanvas.h"
#include <Windows.h>

namespace RRS {
	Button::Button(std::wstring labelStr)
		: labelStr {labelStr}
	{
		SetBackgroundColor(GetColor(88, 28, 156));
		SetBackgroundColorHover(GetColor(28, 88, 156));
		AlignmentHorizontal = 1;
		auto label = std::make_shared<Label>(labelStr);
		label->SetFontColor(GetColor(255, 255, 255));
		Width = label->Width + 50;
		Height = label->Height + 30;
		AddChild(label);
	}
	Button::~Button()
	{

	}
}