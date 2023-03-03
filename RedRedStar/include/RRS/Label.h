#pragma once
#include "Element.h"
#include <string>
class SkFont;
namespace RRS {
	class Label:public Element
	{
		public:
			Label(std::wstring text);
			void Paint(SkCanvas* canvas) override;
			void SetFontColor(Color fontColor);
			std::wstring Text;
	private:
		Color fontColor;
		SkFont* font;
		float advanceX;
		float advanceY;
	};
}


