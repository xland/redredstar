#pragma once
#include "Element.h"
class SkFont;
namespace RRS {
	class Label:public Element
	{
		public:
			Label(const char* text);
			void Paint(SkCanvas* canvas) override;
			const char* Text;
	private:
		SkFont* font;
		float advanceX;
		float advanceY;
	};
}


