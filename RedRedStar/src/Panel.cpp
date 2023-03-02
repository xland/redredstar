#include "../include/RRS/Panel.h"
#include "../include/RRS/Layout.h"
#include "../include/RRS/Element.h"
#include "include/core/SkCanvas.h"
#include "../include/RRS/Window.h"

namespace RRS {
	Panel::Panel()
	{
		AddEventListener(EventType::MouseOver, [this](EventListener* arg) {
			//InvalidateRect(OwnerWindow->Hwnd, nullptr, false);
		});
		AddEventListener(EventType::MouseOut, [this](EventListener* arg) {
			//InvalidateRect(OwnerWindow->Hwnd, nullptr, false);
		});
	}
	Panel::~Panel()
	{
	}
	void Panel::SetBackgroundColor(Color color)
	{
		if (backgroundColor != color) {
			backgroundColor = color;
		}
	}
	void Panel::SetBackgroundColorHover(Color color)
	{
		if (backgroundColorHover != color) 
		{
			backgroundColorHover = color;
		}
	}
	void Panel::Paint(SkCanvas* canvas)
	{
		calculatePosition();
		SkPaint paint;
		paint.setColor(GetIsMouseEnter()?backgroundColor:backgroundColorHover);
		paint.setStrokeJoin(SkPaint::Join::kRound_Join);
		SkRect rect = SkRect::MakeXYWH(xAbsolute, yAbsolute, GetWidth(), GetHeight());
		canvas->drawRoundRect(rect, 12.0, 12.0, paint);
		for (auto element : children)
		{
			element->Paint(canvas);
		}
	}	
	void Panel::SetIsMouseEnter(int x, int y)
	{
		Element::SetIsMouseEnter(x, y);
		for (auto& item : children)
		{
			item->SetIsMouseEnter(x, y);
		}
	}
	void Panel::AddChildElement(std::shared_ptr<Element> element)
	{
		element->SetParentElement(this);
		addLayoutChild(element.get());
		children.push_back(element);
	}
}
