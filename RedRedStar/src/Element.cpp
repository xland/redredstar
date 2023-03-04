#include "../include/RRS/Element.h"
#include "../include/RRS/App.h"
#include "include/core/SkCanvas.h"
#include "../include/RRS/Window.h"
#include <vector>
#include <algorithm>

namespace RRS {
	Element::Element()
	{
	}
	Element::~Element()
	{
	}
	void Element::regMouseHoverEvent()
	{
		if (backgroundColor != backgroundColorHover && hoverId == -1 && hoverOffId == -1) {
			hoverId = AddEventListener(EventType::MouseOver, [this]() {
				InvalidateRect(this->OwnerWindow->Hwnd, nullptr, false);
			});
			hoverOffId = AddEventListener(EventType::MouseOut, [this]() {
				InvalidateRect(this->OwnerWindow->Hwnd, nullptr, false);
			});
		}
		if (backgroundColor == backgroundColorHover && hoverId != -1 && hoverOffId != -1)
		{
			RemoveEventListener(hoverId);
			RemoveEventListener(hoverOffId);
			hoverId = -1;
			hoverOffId = -1;
		}
	}
	void Element::AddChild(std::shared_ptr<Element> child)
	{
		child->ParentElement = this;
		Children.push_back(child);
	}
	void Element::SetBackgroundColor(Color color)
	{
		if (backgroundColor == color) return;
		backgroundColor = color;
		regMouseHoverEvent();
	}
	void Element::SetBackgroundColorHover(Color color)
	{
		if (backgroundColorHover == color) return;
		backgroundColorHover = color;
		regMouseHoverEvent();
	}
	void Element::Paint(SkCanvas* canvas)
	{
		if (isHide) return;
		Color color = IsMouseEnter ? backgroundColorHover : backgroundColor;
		SkPaint paint;
		paint.setColor(color);
		SkRect rect = SkRect::MakeXYWH(X, Y, Width, Height);
		if (BorderRadius != 0.f) {
			canvas->drawRoundRect(rect, BorderRadius, BorderRadius, paint);
		}
		else
		{
			canvas->drawRect(rect, paint);
		}
		for (auto ele : Children)
		{
			ele->Paint(canvas);
		}
	}
	void Element::Show() 
	{
		isHide = false;
	}
	void Element::Hide() 
	{
		isHide = true;
	}
	void Element::SetIsMouseEnter(int x, int y)
	{
		if (!OwnerWindow|| X > OwnerWindow->WidthClient || Y > OwnerWindow->HeightClient || isHide) return;
		bool flag = x > X && y > Y && x < X + Width && y < Y + Height;
		if (!IsMouseEnter && flag) 
		{
			IsMouseEnter = true;
			EmitEvent(RRS::EventType::MouseOver);
		}
		else if(IsMouseEnter && !flag)
		{
			IsMouseEnter = false;
			EmitEvent(RRS::EventType::MouseOut);
		}
		for (auto& ele : Children)
		{
			ele->SetIsMouseEnter(x, y);
		}
	}
	void Element::Click()
	{
		if (!IsMouseEnter) return;
		EmitEvent(RRS::EventType::Click);
		for (auto& ele : Children)
		{
			ele->EmitEvent(RRS::EventType::Click);
		}			
	}
}
