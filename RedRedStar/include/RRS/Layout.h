#pragma once
#include "CommonType.h"
struct YGNode;
struct YGConfig;
namespace RRS {
	class WindowBase;
	class Element;
	enum class LayoutAlign
	{
		Auto,
		FlexStart,
		Center,
		FlexEnd,
		Stretch,
		Baseline,
		SpaceBetween,
		SpaceAround
	};
	enum class FlexDirection
	{
		Column,
		ColumnReverse,
		Row,
		RowReverse
	};
	enum class JustifyContent
	{
		FlexStart,
		Center,
		FlexEnd,
		SpaceBetwee,
		SpaceAroundn,
		SpaceEvenly
	};
	class Layout
	{
	public:
		Layout();
		Layout(YGConfig* config);
		~Layout();
		void SetLayoutPadding(float padding);
		void SetLayoutPadding(float left, float top, float right, float bottom);
		void SetLayoutMargin(float margin);
		void SetLayoutMargin(float left, float top, float right, float bottom);
		void AddChild(Layout* target);
		void SetSize(float w, float h);
		void SetAlignSelf(LayoutAlign align);
		void SetFlexDirection(FlexDirection direction);
		void SetJustifyContent(JustifyContent justifyContent);
		Rectangle GetRectangle();
		void CalculateLayout(float w,float h); //todo 是不是只有window base访问权力才合适
	private:
		friend WindowBase;
		friend Element;
		YGNode* layout;
	};
}


