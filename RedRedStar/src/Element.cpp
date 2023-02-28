#include "../include/RRS/Element.h"
#include "../include/RRS/Layout.h"
#include "../include/RRS/App.h"
#include <ranges>

namespace RRS {
	Element::Element()
		:ParentElement{nullptr} 
		,OwnerWindow {nullptr}
	{
		
	}
	Element::~Element()
	{
	}
	void Element::Show() {
	}
	void Element::Hide() {
		//todo
	}
	void Element::calculatePosition()
	{
		if (ParentElement) {
			xAbsolute = ParentElement->xAbsolute + GetXOffset();
			yAbsolute = ParentElement->yAbsolute + GetYOffset();
		}
		else
		{
			xAbsolute = GetXOffset();
			yAbsolute = GetYOffset();
		}
	}
	bool Element::GetIsMouseEnter()
	{
		return isMouseEnter;
	}
	void Element::SetIsMouseEnter(int x, int y)
	{
		bool flag = x > xAbsolute && y > yAbsolute && x < xAbsolute + GetWidth() && y < yAbsolute + GetHeight();
		if (!isMouseEnter && flag) {
			isMouseEnter = true;
			EmitEvent(EventType::MouseOver);
		}
		else if(isMouseEnter && !flag)
		{
			isMouseEnter = false;
			EmitEvent(EventType::MouseOut);
		}
	}
}
