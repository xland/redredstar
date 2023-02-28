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
		virtual void SetSize(float w, float h);
		virtual float GetWidth();
		virtual float GetHeight();
		virtual void SetWidth(float w);
		virtual void SetHeight(float h);
		float GetXOffset();
		float GetYOffset();
		float GetXAbsolute();
		float GetYAbsolute();
		void SetAlignSelf(LayoutAlign align);
		void SetFlexDirection(FlexDirection direction);
		void SetJustifyContent(JustifyContent justifyContent);
	protected:
		Layout();
		~Layout();
		void addLayoutChild(std::shared_ptr<Element> target);
		YGNode* layout;
		float xAbsolute = 0;
		float yAbsolute = 0;
	};
}


