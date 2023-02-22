#include "include/core/SkCanvas.h"
#include "../include/RRS/Element.h"

namespace RRS {
	Element::Element()
	{
	}
	Element::~Element()
	{
	}
	void Element::Paint(SkCanvas* canvas)
	{
		SkPaint paint;
		paint.setColor(SkColorSetARGB(255, 220, 220, 220));
		paint.setStrokeJoin(SkPaint::Join::kRound_Join);
		SkRect rect = SkRect::MakeXYWH(20, 20, 180, 50);
		canvas->drawRoundRect(rect, 12, 32, paint);
		for (auto element : Children)
		{
			element->Paint(canvas);
		}
	}
	void Element::Show() {
	}
	void Element::Hide() {
		//todo
	}
}
