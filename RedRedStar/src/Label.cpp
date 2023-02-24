#include "include/core/SkFont.h"
#include "include/core/SkCanvas.h"
#include "../include/RRS/Label.h"
#include "../include/RRS/Layout.h"
#include "modules/skparagraph/include/TextStyle.h"
namespace RRS {
	Label::Label(const char* text):Text {text},font{new SkFont(nullptr,40)}
	{
		SkRect rect;
		SkString str{ Text };
		font->measureText(Text, strlen(Text), SkTextEncoding::kUTF8, &rect);
		font->setSubpixel(true);
		advanceX = rect.x();
		advanceY = -rect.y();
		Layout->SetSize(rect.width(), rect.height());
	}
	void Label::Paint(SkCanvas* canvas)
	{		
		auto layoutRect = Layout->GetRectangle();
		canvas->translate(layoutRect.X, layoutRect.Y);
		SkPaint paint;
		paint.setColor(SK_ColorWHITE);
		canvas->drawString(Text, advanceX, advanceY, *font, paint);
	}
}