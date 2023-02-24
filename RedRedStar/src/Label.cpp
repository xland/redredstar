#include "include/core/SkFont.h"
#include "include/core/SkCanvas.h"
#include "../include/RRS/Label.h"
#include "../include/RRS/Layout.h"
#include "modules/skparagraph/include/TextStyle.h"
namespace RRS {
	Label::Label(const char* text):Text {text},font{new SkFont(nullptr,20)}
	{
		SkRect rect;
		SkString str{ Text };
		font->setSubpixel(true);
		font->measureText(Text, strlen(Text), SkTextEncoding::kUTF8, &rect);
		Layout->SetSize(rect.width(), rect.height());
	}
	void Label::Paint(SkCanvas* canvas)
	{
		SkPaint paint;
		paint.setColor(BackgroundColor);
		paint.setStrokeJoin(SkPaint::Join::kRound_Join);
		auto layoutRect = Layout->GetRectangle();
		paint.setColor(SK_ColorBLACK);		
		canvas->drawString(Text, layoutRect.X, layoutRect.Y, *font, paint);
	}
}