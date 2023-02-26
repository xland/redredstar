#include "../include/RRS/Element.h"
#include "../include/RRS/Layout.h"
#include "../include/RRS/App.h"
#include "../include/RRS/Window.h"
#include <ranges>

namespace RRS {
	Element::Element()
		:ParentElement{nullptr} ,BackgroundColor { RRS::GetColor(255, 255, 255, 255) }
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
		BackgroundColor = color;
	}

	Window* Element::GetOwnerWindow()
	{
		Window* result = nullptr;
		Element* ele = this->ParentElement;
		while (ele)
		{
			ele = ele->ParentElement;
		}
		for (auto win: App::Get()->Windows)
		{
			for (auto eleItem : win->Children)
			{
				if (eleItem == ele) 
				{
					result = win;
					break;
				}
			}
			if (result) break;
		}
		return result;
	}
}
