#pragma once
#include "LayoutEnum.h"
#include <vector>
#include <memory>

namespace RRS {
	class Element;
	class Layout
	{
	public:
		Layout() = default;
		~Layout() = default;
		void CaculateLayout();
		void  SetXAbsolute(float xAbsolute);
		void  SetYAbsolute(float yAbsolute);
		void  SetWidthPercent(float widthPercent);
		void  SetHeightPercent(float heightPercent);
		void  SetflexDirection(FlexDirection flexDirection);
		void  SetAlignVertical(Align alignVertical);
		void  SetAlignHorizontal(Align alignHorizontal);
		void SetMarginLeft(float marginLeft);
		void SetMarginRight(float marginRight);
		void SetMarginTop(float marginTop);
		void SetMarginBottom(float marginBottom);
		void SetPaddingLeft(float paddingLeft);
		void SetPaddingRight(float paddingRight);
		void SetPaddingTop(float paddingTop);
		void SetPaddingBottom(float paddingBottom);
		void  SetFlex(float flex);
		virtual void  SetDirty(bool flag);

		float  GetXAbsolute();
		float  GetYAbsolute();
		float  GetWidthPercent();
		float  GetHeightPercent();
		FlexDirection  GetflexDirection();
		Align  GetAlignVertical();
		Align  GetAlignHorizontal();

		float GetMarginLeft();
		float GetMarginRight();
		float GetMarginTop();
		float GetMarginBottom();
		float GetPaddingLeft();
		float GetPaddingRight();
		float GetPaddingTop();
		float GetPaddingBottom();

		float  GetFlex();
		bool  GetDirty();

		virtual void SetWidth(float width) = 0;
		virtual void SetHeight(float height) = 0;
		virtual float GetWidth() = 0;
		virtual float GetHeight() = 0;

		std::vector<std::shared_ptr<Element>> Children;
	private:
		void caculateAlignHorizontal();
		void caculateAlignVertical();
		float xAbsolute = 0.f;
		float yAbsolute = 0.f;
		float widthPercent = 0.f;
		float heightPercent = 0.f;
		float marginLeft = 0.f;
		float marginRight = 0.f;
		float marginTop = 0.f;
		float marginBottom = 0.f;
		float paddingLeft = 0.f;
		float paddingRight = 0.f;
		float paddingTop = 0.f;
		float paddingBottom = 0.f;
		bool useWidthPercent = false;
		bool useHeightPercent = false;
		FlexDirection flexDirection = FlexDirection::Row;
		Align alignVertical = Align::Start;
		Align alignHorizontal = Align::Start;
		float flex = 0.f;
		bool isDirty = true;
	};
}


