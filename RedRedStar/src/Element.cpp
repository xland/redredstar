#include "../include/RRS/Element.h"
#include "../include/RRS/Layout.h"
#include "../include/RRS/App.h"
#include <ranges>

namespace RRS {
	Element::Element()
		:parentElement{nullptr}
		,ownerWindow {nullptr}
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
	void Element::EmitClickEvent()
	{
		if (isMouseEnter) {
			EmitEvent(RRS::EventType::Click);
		}
	}
	void Element::CalculatePosition()
	{
		if (parentElement) {
			xAbsolute = parentElement->xAbsolute + GetXOffset();
			yAbsolute = parentElement->yAbsolute + GetYOffset();
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
	void Element::SetParentElement(Element* element)
	{
		parentElement = element;
	}
	Window* Element::GetOwnerWindow()
	{
		Window* result = ownerWindow;
		if (!result) {
			auto parent = parentElement;
			while (!result && parent)
			{
				result = parent->ownerWindow;
				parent = parent->parentElement;
			}
			ownerWindow = result;
		}
		return result;
	}

	Element* Element::GetParentElement()
	{
		return parentElement;
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
