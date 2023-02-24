#include "../include/RRS/Panel.h"
#include "include/core/SkCanvas.h"
#include "../include/RRS/Layout.h"

namespace RRS {
	Panel::Panel()
	{

	}
	void Panel::Paint(SkCanvas* canvas)
	{
		SkPaint paint;
		paint.setColor(BackgroundColor);
		paint.setStrokeJoin(SkPaint::Join::kRound_Join);
		auto layoutRect = Layout->GetRectangle();
		SkRect rect = SkRect::MakeXYWH(layoutRect.X, layoutRect.Y, layoutRect.W, layoutRect.H);
		canvas->drawRoundRect(rect, 12, 12, paint);
		for (auto element : Children)
		{
			element->Paint(canvas);
		}
	}

	void Panel::AddElement(Element* element)
	{
		element->ParentElement = this;
		Layout->AddChild(element->Layout);
		Children.push_back(element);
	}
}
