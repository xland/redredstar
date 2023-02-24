#include "../include/RRS/Element.h"
#include "../include/RRS/Layout.h"
#include "../include/RRS/App.h"
#include "../include/RRS/WindowBase.h"
#include <ranges>

namespace RRS {
	Element::Element()
		:ParentElement{nullptr}, Layout{new RRS::Layout()}
		,BackgroundColor { RRS::GetColor(255, 255, 255, 255) }
	{
		
	}
	Element::~Element()
	{
		delete Layout;
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

	WindowBase* Element::GetOwnerWindow()
	{
		WindowBase* result = nullptr;
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
