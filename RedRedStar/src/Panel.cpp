#include "../include/RRS/Panel.h"
#include "include/core/SkCanvas.h"
#include "../include/RRS/Layout.h"
#include "../include/RRS/Element.h"

namespace RRS {
	Panel::Panel()
	{

	}
	Panel::~Panel()
	{
	}
	void Panel::Paint(SkCanvas* canvas)
	{
		calculatePosition();
		SkPaint paint;
		paint.setColor(BackgroundColor);
		paint.setStrokeJoin(SkPaint::Join::kRound_Join);
		SkRect rect = SkRect::MakeXYWH(xAbsolute, yAbsolute, GetWidth(), GetHeight());
		canvas->drawRoundRect(rect, 12.0, 12.0, paint);
		for (auto element : Children)
		{
			element->Paint(canvas);
		}
	}	
	void Panel::CheckMousePosition(int x, int y)
	{
		Element::CheckMousePosition(x, y);
		for (auto& item : Children)
		{
			item->CheckMousePosition(x, y);
		}
	}
	void Panel::AddElement(std::shared_ptr<Element> element)
	{
		element->OwnerWindow = this->OwnerWindow;
		element->ParentElement = this;
		addLayoutChild(element);
		Children.push_back(element);
	}
}
