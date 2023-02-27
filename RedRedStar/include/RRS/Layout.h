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
		Rectangle GetRectangle();
	protected:
		Layout();
		~Layout();
		void addLayoutChild(std::shared_ptr<Element> target);
		void calculateLayout(float w, float h); //todo 是不是只有window base访问权力才合适
	private:
		YGNode* layout;
	};
}


