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
		if (!IsMouseEnter) {
			SetDirty(true);
		}		
	}
	void Element::SetBackgroundColorHover(Color color)
	{
		backgroundColorHover = color;
		if (IsMouseEnter) {
			SetDirty(true);
		}
	}
	void Element::SetDirty(bool flag) {
		Layout::SetDirty(flag);
		if (!OwnerWindow) return;
		OwnerWindow->SetDirty(flag);
	}
	void Element::drawBorder(SkPaint& paint, SkCanvas* canvas, SkRect& rect)
	{
		paint.setStyle(SkPaint::kStroke_Style);
		auto x = rect.x();
		auto y = rect.y();
		auto right = rect.right();
		auto bottom = rect.bottom();
		auto border = GetBorderTop();
		if (border > 0) {
			paint.setStrokeWidth(border);
			paint.setColor(GetBorderTopColor());
			auto temp = y + border / 2;
			canvas->drawLine({ x,temp }, { right,temp }, paint);
		}
		border = GetBorderRight();
		if (border > 0) {
			paint.setStrokeWidth(border);
			paint.setColor(GetBorderRightColor());
			auto temp = x + rect.width() - border / 2;
			canvas->drawLine({ temp,y }, { temp,bottom }, paint);
		}
		border = GetBorderBottom();
		if (border > 0) {
			paint.setStrokeWidth(border);
			paint.setColor(GetBorderBottomColor());
			auto temp = bottom - border / 2;
			canvas->drawLine({ x,temp }, { right,temp }, paint);
		}
		border = GetBorderLeft();
		if (border > 0) {
			paint.setStrokeWidth(border);
			paint.setColor(GetBorderLeftColor());
			auto temp = x + border / 2;
			canvas->drawLine({ temp,y }, { temp,bottom }, paint);
		}
	}
	Color Element::GetCurrentBackgroundColor()
	{
		Color color = backgroundColor;
		if (IsMouseEnter && backgroundColorHover != UINT32_MAX) {
			color = backgroundColorHover;
		}
		return color;
	}
	void Element::Paint(SkCanvas* canvas)
	{
		if (IsOutOfView()) return;
		if (GetDirty()) {	
			CaculateLayout();			
			Color color = GetCurrentBackgroundColor();
			SkPaint paint;
			paint.setAntiAlias(true);
			paint.setColor(color);
			SkRect rect = SkRect::MakeXYWH(GetXAbsolute(), GetYAbsolute(), GetWidthReal(), GetHeightReal());
			if (BorderRadius != 0.f) 
			{
				canvas->drawRoundRect(rect, BorderRadius, BorderRadius, paint);
			}
			else
			{
				canvas->drawRect(rect, paint);
			}
			drawBorder(paint, canvas, rect);
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
		auto flag = !OwnerWindow || isHide || x > OwnerWindow->GetWidthReal() || y > OwnerWindow->GetHeightReal() ;
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
			OnMouseEnter();
			EmitEvent(RRS::EventType::MouseOver);
			if (backgroundColorHover != UINT32_MAX &&  backgroundColor != backgroundColorHover) {
				SetDirty(true);
			}
		}
		else if(IsMouseEnter && !flag)
		{
			IsMouseEnter = false;
			OnMouseOut();
			EmitEvent(RRS::EventType::MouseOut);
			if (backgroundColorHover != UINT32_MAX && backgroundColor != backgroundColorHover) {
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
		OnClick();
		EmitEvent(RRS::EventType::Click);
		for (auto& ele : Children)
		{
			ele->Click();
		}
	}
}
