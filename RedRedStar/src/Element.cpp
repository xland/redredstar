#include "../include/RRS/Element.h"
#include "../include/RRS/Layout.h"
#include "../include/RRS/App.h"
#include "../include/RRS/Window.h"
#include <ranges>

namespace RRS {
	Element::Element()
		:ParentElement{nullptr} 
		,BackgroundColor { RRS::GetColor(255, 255, 255, 255) }
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
	void Element::SetBackgroundColor(Color color) 
	{
		if (color != BackgroundColor) {
			BackgroundColor = color;
			if (OwnerWindow) {
				InvalidateRect(OwnerWindow->Hwnd, nullptr, false);
			}			
		}
		
	}
	void Element::calculatePosition()
	{
		calculateOffsetPosition();
		if (ParentElement) {
			XAbsolute = ParentElement->XAbsolute + XOffset;
			YAbsolute = ParentElement->YAbsolute + YOffset;
		}
		else
		{
			XAbsolute = XOffset;
			YAbsolute = YOffset;
		}
	}
	void Element::CheckMousePosition(int x, int y)
	{
		bool flag = x > XAbsolute && y > YAbsolute && x < XAbsolute + Width && y < YAbsolute + Height;
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
