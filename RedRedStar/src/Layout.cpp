#include "../include/RRS/Layout.h"
#include "../include/RRS/Element.h"
#include <Yoga.h>
#include <memory>
namespace RRS {
	Layout::Layout() :layout{ YGNodeNew() }
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
	void Layout::addLayoutChild(std::shared_ptr<Element> target)
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
	void Layout::SetFlexDirection(FlexDirection direction)
	{
		YGNodeStyleSetFlexDirection(layout, (YGFlexDirection)direction);
	}
	void Layout::SetJustifyContent(JustifyContent justifyContent)
	{
		YGNodeStyleSetJustifyContent(layout, (YGJustify)justifyContent);
	}
	void Layout::calculateLayout()
	{
		
	}
	void Layout::calculateOffsetPosition()
	{
		XOffset = YGNodeLayoutGetLeft(layout);
		YOffset = YGNodeLayoutGetTop(layout);
		Width = YGNodeLayoutGetWidth(layout);
		Height = YGNodeLayoutGetHeight(layout);
	}
}