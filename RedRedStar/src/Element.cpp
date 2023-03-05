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
		//if (backgroundColor != backgroundColorHover && hoverId == -1 && hoverOffId == -1) {
		//	hoverId = AddEventListener(EventType::MouseOver, [this]() {
		//		InvalidateRect(this->OwnerWindow->Hwnd, nullptr, false);
		//	});
		//	hoverOffId = AddEventListener(EventType::MouseOut, [this]() {
		//		InvalidateRect(this->OwnerWindow->Hwnd, nullptr, false);
		//	});
		//}
		//if (backgroundColor == backgroundColorHover && hoverId != -1 && hoverOffId != -1)
		//{
		//	RemoveEventListener(hoverId);
		//	RemoveEventListener(hoverOffId);
		//	hoverId = -1;
		//	hoverOffId = -1;
		//}
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
	void Element::SetDirty(bool flag) {
		Layout::SetDirty(flag);
		if (!OwnerWindow) return;
		OwnerWindow->SetDirty(flag);
	}
	void Element::SetWidth(float width)
	{
		this->width = width;
		SetDirty(true);
	}
	void Element::SetHeight(float height)
	{
		this->height = height;
		SetDirty(true);
	}

	float Element::GetWidth() {
		return width;
	}
	float Element::GetHeight() {
		return height;
	}
	void Element::CaculateLayout()
	{
		Layout* parent = ParentElement;
		if (!parent) parent = OwnerWindow;
		if (parent->GetAlignHorizontal() == Align::Center) {
			auto x = (parent->GetWidth() - width) / 2 + parent->GetXAbsolute();
			SetXAbsolute(x);
		}
		if (parent->GetAlignVertical() == Align::Center) {
			auto y = (parent->GetHeight() - height) / 2 + parent->GetYAbsolute();
			SetYAbsolute(y);
		}
	}
	void Element::Paint(SkCanvas* canvas)
	{
		CaculateLayout();
		if (IsOutOfView()) return;
		if (GetDirty()) {				
			Color color = IsMouseEnter ? backgroundColorHover : backgroundColor;
			SkPaint paint;
			paint.setColor(color);
			auto x = GetXAbsolute();
			auto y = GetYAbsolute();
			SkRect rect = SkRect::MakeXYWH(x, y, width, height);
			if (BorderRadius != 0.f) 
			{
				canvas->drawRoundRect(rect, BorderRadius, BorderRadius, paint);
			}
			else
			{
				canvas->drawRect(rect, paint);
			}
			SetDirty(false);
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
	bool Element::IsOutOfView()
	{
		auto x = GetXAbsolute();
		auto y = GetYAbsolute();
		auto flag = !OwnerWindow || isHide || x > OwnerWindow->GetWidth() || y > OwnerWindow->GetHeight() ;
		return flag;
	}
	void Element::SetIsMouseEnter(int x, int y)
	{
		if (IsOutOfView()) return;
		auto X = GetXAbsolute();
		auto Y = GetYAbsolute();
		bool flag = x > X && y > Y && x < X + width && y < Y + height;
		if (!IsMouseEnter && flag) 
		{
			IsMouseEnter = true;
			EmitEvent(RRS::EventType::MouseOver);
			if (backgroundColor != backgroundColorHover) {
				SetDirty(true);
			}
		}
		else if(IsMouseEnter && !flag)
		{
			IsMouseEnter = false;
			EmitEvent(RRS::EventType::MouseOut);
			if (backgroundColor != backgroundColorHover) {
				SetDirty(true);
			}
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
