#include "../include/RRS/Panel.h"
#include "../include/RRS/Layout.h"
#include "../include/RRS/Element.h"
#include "include/core/SkCanvas.h"
#include "../include/RRS/Window.h"

namespace RRS {
	Panel::Panel()
		:backgroundColor { GetColor(255, 255, 255) }
		,backgroundColorHover { GetColor(255, 255, 255) }
	{

	}
	Panel::~Panel()
	{
	}
	void Panel::regMouseHoverEvent()
	{
		if (backgroundColor != backgroundColorHover && hoverId == -1 && hoverOffId == -1) {
			hoverId = AddEventListener(EventType::MouseOver, [this](EventListener* arg) {
				InvalidateRect(this->GetOwnerWindow()->Hwnd, nullptr, false);
				});
			hoverOffId = AddEventListener(EventType::MouseOut, [this](EventListener* arg) {
				InvalidateRect(this->GetOwnerWindow()->Hwnd, nullptr, false);
				});
		}
		if (backgroundColor == backgroundColorHover && hoverId != -1 && hoverOffId != -1)
		{
			RemoveEventListener(EventType::MouseOver, hoverId);
			RemoveEventListener(EventType::MouseOver, hoverOffId);
			hoverId = -1;
			hoverOffId = -1;
		}
	}
	void Panel::SetBackgroundColor(Color color)
	{
		if (backgroundColor != color) 
		{
			backgroundColor = color;
			regMouseHoverEvent();
		}
		
	}
	void Panel::SetBackgroundColorHover(Color color)
	{
		if (backgroundColorHover != color) 
		{
			backgroundColorHover = color;
			regMouseHoverEvent();
		}
	}

	void Panel::SetBorderRadius(float borderRadius)
	{
		this->borderRadius = borderRadius;
	}
	void Panel::Paint(SkCanvas* canvas)
	{
		calculatePosition();
		SkPaint paint;
		paint.setAntiAlias(true);
		paint.setColor(GetIsMouseEnter()?backgroundColor:backgroundColorHover);
		paint.setStrokeJoin(SkPaint::Join::kRound_Join);
		SkRect rect = SkRect::MakeXYWH(xAbsolute, yAbsolute, GetWidth(), GetHeight());
		if (borderRadius != 0.f) {
			canvas->drawRoundRect(rect, borderRadius, borderRadius, paint);
		}
		else
		{
			canvas->drawRect(rect, paint);
		}
		
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
