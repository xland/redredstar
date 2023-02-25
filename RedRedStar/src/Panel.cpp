#include "../include/RRS/Panel.h"
#include "include/core/SkCanvas.h"
#include "../include/RRS/Layout.h"

namespace RRS {
	Panel::Panel()
	{

	}
	void Panel::Paint(SkCanvas* canvas)
	{
		auto layoutRect = GetRectangle();
		canvas->translate(layoutRect.X, layoutRect.Y);
		SkPaint paint;
		paint.setColor(BackgroundColor);
		paint.setStrokeJoin(SkPaint::Join::kRound_Join);
		SkRect rect = SkRect::MakeXYWH(0, 0, layoutRect.W, layoutRect.H);
		canvas->drawRoundRect(rect, 12.0, 12.0, paint);
		for (auto element : Children)
		{
			element->Paint(canvas);
		}
	}

	void Panel::AddElement(Element* element)
	{
		element->ParentElement = this;
		addLayoutChild(element);
		Children.push_back(element);
	}
}
