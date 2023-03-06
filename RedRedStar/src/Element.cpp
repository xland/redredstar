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
	void Element::AddChild(std::shared_ptr<Element> child)
	{
		child->ParentElement = this;
		Children.push_back(child);
	}
	void Element::SetBackgroundColor(Color color)
	{
		backgroundColor = color;
	}
	void Element::SetBackgroundColorHover(Color color)
	{
		backgroundColorHover = color;
	}
	void Element::SetDirty(bool flag) {
		Layout::SetDirty(flag);
		if (!OwnerWindow) return;
		OwnerWindow->SetDirty(flag);
	}
	void Element::Paint(SkCanvas* canvas)
	{
		if (IsOutOfView()) return;
		if (GetDirty()) {	
			CaculateLayout();
			Color color = IsMouseEnter ? backgroundColorHover : backgroundColor;
			SkPaint paint;
			paint.setColor(color);
			auto x = GetXAbsolute();
			auto y = GetYAbsolute();
			SkRect rect = SkRect::MakeXYWH(x, y, GetWidthReal(), GetHeightReal());
			if (BorderRadius != 0.f) 
			{
				canvas->drawRoundRect(rect, BorderRadius, BorderRadius, paint);
			}
			else
			{
				canvas->drawRect(rect, paint);
			}
			paint.setStyle(SkPaint::kStroke_Style);
			auto border = GetBorderTop();
			if (border > 0) {
				paint.setStrokeWidth(border);
				paint.setColor(GetBorderTopColor());
				canvas->drawLine({ x,y }, { x + rect.width(),y}, paint);
			}
			border = GetBorderRight();
			if (border > 0) {
				paint.setStrokeWidth(border);
				paint.setColor(GetBorderRightColor());
				canvas->drawLine({ x + rect.width()-border,y }, { x + rect.width()-border,y+rect.height()}, paint);
			}
			border = GetBorderBottom();
			if (border > 0) {
				paint.setStrokeWidth(border);
				paint.setColor(GetBorderBottomColor());
				canvas->drawLine({ x,y+rect.height()-border}, {x + rect.width(),y + rect.height() - border }, paint);
			}
			border = GetBorderLeft();
			if (border > 0) {
				paint.setStrokeWidth(border);
				paint.setColor(GetBorderLeftColor());
				canvas->drawLine({ x,y }, { x,y+rect.height()}, paint);
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
		bool flag = x > X && y > Y && x < X + GetWidthReal() && y < Y + GetHeightReal();
		if (!IsMouseEnter && flag) 
		{
			IsMouseEnter = true;
			if (backgroundColor != backgroundColorHover) {
				SetDirty(true);
			}
			EmitEvent(RRS::EventType::MouseOver);
		}
		else if(IsMouseEnter && !flag)
		{
			IsMouseEnter = false;
			if (backgroundColor != backgroundColorHover) {
				SetDirty(true);
			}
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
