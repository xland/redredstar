#include "include/core/SkCanvas.h"
#include "../include/RRS/Element.h"
#include <Yoga.h>

namespace RRS {
	Element::Element():Layout{ YGNodeNew() },OwnerWindow {nullptr},ParentElement{nullptr}
	{
		YGNodeStyleSetWidth(Layout, Width);
		YGNodeStyleSetHeight(Layout, Height);
		YGNodeStyleSetAlignSelf(Layout, YGAlignCenter);
	}
	Element::~Element()
	{
		YGNodeFreeRecursive(Layout);
	}
	void Element::Paint(SkCanvas* canvas)
	{
		SkPaint paint;
		paint.setColor(SkColorSetARGB(255, 220, 220, 220));
		paint.setStrokeJoin(SkPaint::Join::kRound_Join);
		float x= YGNodeLayoutGetLeft(Layout), y= YGNodeLayoutGetTop(Layout), w= YGNodeLayoutGetWidth(Layout), h= YGNodeLayoutGetHeight(Layout);		
		SkRect rect = SkRect::MakeXYWH(x, y, w, h);
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
		YGNodeInsertChild(Layout, element->Layout, Children.size());
		Children.push_back(element);
	}
	void Element::SetLayoutPadding(int padding)
	{
		YGNodeStyleSetPadding(Layout, YGEdgeAll, padding);
	}
	void Element::SetLayoutPadding(int left, int top, int right, int bottom)
	{
		YGNodeStyleSetPadding(Layout, YGEdgeLeft, left);
		YGNodeStyleSetPadding(Layout, YGEdgeTop, top);
		YGNodeStyleSetPadding(Layout, YGEdgeRight, right);
		YGNodeStyleSetPadding(Layout, YGEdgeBottom, bottom);
	}
	void Element::SetLayoutMargin(int margin)
	{
		YGNodeStyleSetMargin(Layout, YGEdgeAll, margin);
	}
	void Element::SetLayoutMargin(int left, int top, int right, int bottom)
	{
		YGNodeStyleSetMargin(Layout, YGEdgeLeft, left);
		YGNodeStyleSetMargin(Layout, YGEdgeTop, top);
		YGNodeStyleSetMargin(Layout, YGEdgeRight, right);
		YGNodeStyleSetMargin(Layout, YGEdgeBottom, bottom);
	}
	void Element::SetBackgroundColor(unsigned r, unsigned g, unsigned b, unsigned a) 
	{
		BackgroundColor = (a << 24) | (r << 16) | (g << 8) | (b << 0);
	}
}
