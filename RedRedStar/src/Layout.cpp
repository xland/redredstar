#include "../include/RRS/Layout.h"
#include <Yoga.h>
namespace RRS {
	Layout::Layout() :layout{ YGNodeNew() }
	{
	}
	Layout::Layout(YGConfig* config) : layout{ YGNodeNewWithConfig(config) }
	{

	}
	Layout::~Layout()
	{
		YGNodeFreeRecursive(layout);
	}
	void Layout::SetLayoutPadding(float padding)
	{
		YGNodeStyleSetPadding(layout, YGEdgeAll, padding);
	}
	void Layout::SetLayoutPadding(float left, float top, float right, float bottom)
	{
		YGNodeStyleSetPadding(layout, YGEdgeLeft, left);
		YGNodeStyleSetPadding(layout, YGEdgeTop, top);
		YGNodeStyleSetPadding(layout, YGEdgeRight, right);
		YGNodeStyleSetPadding(layout, YGEdgeBottom, bottom);
	}
	void Layout::SetLayoutMargin(float margin)
	{
		YGNodeStyleSetMargin(layout, YGEdgeAll, margin);
	}
	void Layout::SetLayoutMargin(float left, float top, float right, float bottom)
	{
		YGNodeStyleSetMargin(layout, YGEdgeLeft, left);
		YGNodeStyleSetMargin(layout, YGEdgeTop, top);
		YGNodeStyleSetMargin(layout, YGEdgeRight, right);
		YGNodeStyleSetMargin(layout, YGEdgeBottom, bottom);
	}
	void Layout::addLayoutChild(Layout* target)
	{
		auto index = YGNodeGetChildCount(layout);
		YGNodeInsertChild(layout, target->layout, index);
	}
	void Layout::SetSize(float w, float h)
	{
		YGNodeStyleSetWidth(layout, w);
		YGNodeStyleSetHeight(layout, h);
	}
	void Layout::SetAlignSelf(LayoutAlign align)
	{
		YGNodeStyleSetAlignSelf(layout, (YGAlign)align);
	}
	Rectangle Layout::GetRectangle()
	{
		Rectangle rect;
		rect.X = YGNodeLayoutGetLeft(layout); 
		rect.Y = YGNodeLayoutGetTop(layout);
		rect.W = YGNodeLayoutGetWidth(layout);
		rect.H = YGNodeLayoutGetHeight(layout);
		return rect;
	}
	void Layout::SetFlexDirection(FlexDirection direction)
	{
		YGNodeStyleSetFlexDirection(layout, (YGFlexDirection)direction);
	}
	void Layout::SetJustifyContent(JustifyContent justifyContent)
	{
		YGNodeStyleSetJustifyContent(layout, (YGJustify)justifyContent);
	}
	void Layout::calculateLayout(float w, float h)
	{
		YGNodeCalculateLayout(layout, w, h, YGDirectionLTR);
	}
}