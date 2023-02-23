#include "../include/RRS/WindowBase.h"
#include <Yoga.h>

namespace RRS {
	void WindowBase::AddElement(Element* element)
	{
		element->OwnerWindow = this;
		YGNodeInsertChild(layout, element->Layout, Children.size());
		Children.push_back(element);
	}
	void WindowBase::initLayout() 
	{
		YGNodeStyleSetFlexDirection(layout, YGFlexDirectionColumn);
		YGNodeStyleSetJustifyContent(layout, YGJustifyCenter);
	}
	void WindowBase::disposeLayout()
	{
		YGNodeFreeRecursive(layout);
		YGConfigFree(layoutConfig);
	}
	void WindowBase::calculateLayout() {
		YGNodeCalculateLayout(layout, Width, Height, YGDirectionLTR);
	}
	void WindowBase::SetLayoutPadding(int padding)
	{
		YGNodeStyleSetPadding(layout, YGEdgeAll, padding);
	}
	void WindowBase::SetLayoutPadding(int left, int top, int right, int bottom)
	{
		YGNodeStyleSetPadding(layout, YGEdgeLeft, left);
		YGNodeStyleSetPadding(layout, YGEdgeTop, top);
		YGNodeStyleSetPadding(layout, YGEdgeRight, right);
		YGNodeStyleSetPadding(layout, YGEdgeBottom, bottom);
	}
	void WindowBase::SetLayoutMargin(int margin)
	{
		YGNodeStyleSetMargin(layout, YGEdgeAll, 20);
	}
	void WindowBase::SetLayoutMargin(int left, int top, int right, int bottom)
	{
		YGNodeStyleSetMargin(layout, YGEdgeLeft, left);
		YGNodeStyleSetMargin(layout, YGEdgeTop, top);
		YGNodeStyleSetMargin(layout, YGEdgeRight, right);
		YGNodeStyleSetMargin(layout, YGEdgeBottom, bottom);
	}
}
