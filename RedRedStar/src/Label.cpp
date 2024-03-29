#include "include/core/SkFont.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkFontMgr.h"
#include "../include/RRS/Label.h"
#include "modules/skparagraph/include/TextStyle.h"
namespace RRS {
Label::Label(std::wstring text)
	:Text {text}
	,font{new SkFont(SkTypeface::MakeFromName("Microsoft YaHei", SkFontStyle::Normal()),22)} //Microsoft Yahei
	,fontColor { GetColor(0, 0, 0) }
{
	SkRect rect;
	font->measureText(Text.data(), wcslen(Text.data()) * 2, SkTextEncoding::kUTF16, &rect); //strlen(Text)
	advanceX = rect.x();
	advanceY = -rect.y();
	SetWidth(rect.width());
	SetHeight(rect.height());
}
void Label::SetFontColor(Color fontColor)
{
	this->fontColor = fontColor;
}
void Label::Paint(SkCanvas* canvas)
{
	CaculateLayout();
	SkPaint paint;
	paint.setColor(fontColor);
	paint.setAntiAlias(true);
	//canvas->drawString(SkString{ Text }, xAbsolute + advanceX, yAbsolute + advanceY, *font, paint);		
	canvas->drawSimpleText(Text.data(), wcslen(Text.data()) * 2, SkTextEncoding::kUTF16, GetXAbsolute() + advanceX, GetYAbsolute() + advanceY, *font, paint);
	SetDirty(false);
}
}