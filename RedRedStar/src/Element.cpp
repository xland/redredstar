#include "include/core/SkCanvas.h"
#include "../include/RRS/Element.h"
#include "../include/RRS/Layout.h"

namespace RRS {
	Element::Element():OwnerWindow {nullptr},ParentElement{nullptr}, Layout{new RRS::Layout()}
	{
		
	}
	Element::~Element()
	{
		delete Layout;
	}
	void Element::Paint(SkCanvas* canvas)
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
	void Element::Show() {
	}
	void Element::Hide() {
		//todo
	}
	void Element::AddElement(Element* element)
	{
		element->OwnerWindow = OwnerWindow;
		element->ParentElement = this;
		Layout->AddChild(element->Layout);
		Children.push_back(element);
	}
	
	void Element::SetBackgroundColor(Color color) 
	{
		BackgroundColor = color;
	}
}
