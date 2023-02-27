#pragma once
#include "CommonType.h"
#include "LayoutEnum.h"
#include <memory>
struct YGNode;
struct YGConfig;
namespace RRS {
	class Element;
	class Layout
	{
	public:
		void SetLayoutPadding(float padding);
		void SetLayoutPadding(float left, float top, float right, float bottom);
		void SetLayoutMargin(float margin);
		void SetLayoutMargin(float left, float top, float right, float bottom);
		void SetSize(float w, float h);
		void SetAlignSelf(LayoutAlign align);
		void SetFlexDirection(FlexDirection direction);
		void SetJustifyContent(JustifyContent justifyContent);
		float XAbsolute,YAbsolute,XOffset,YOffset,Width,Height;
	protected:
		Layout();
		~Layout();
		void addLayoutChild(std::shared_ptr<Element> target);
		void calculateLayout(); //todo 是不是只有window base访问权力才合适
		void calculateOffsetPosition();
		YGNode* layout;
	};
}


