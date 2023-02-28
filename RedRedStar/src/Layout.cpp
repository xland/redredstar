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
	void Layout::SetWidth(float w)
	{
		YGNodeStyleSetWidth(layout, w);
	}
	void Layout::SetHeight(float h)
	{
		YGNodeStyleSetHeight(layout, h);
	}
	float Layout::GetWidth()
	{
		return YGNodeStyleGetWidth(layout).value;
	}
	float Layout::GetHeight()
	{
		return YGNodeStyleGetHeight(layout).value;
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
	float Layout::GetXOffset()
	{
		return YGNodeLayoutGetLeft(layout);
	}
	float Layout::GetYOffset()
	{
		return YGNodeLayoutGetTop(layout);
	}

	float Layout::GetXAbsolute()
	{
		return xAbsolute;
	}
	float Layout::GetYAbsolute()
	{
		return yAbsolute;
	}
}