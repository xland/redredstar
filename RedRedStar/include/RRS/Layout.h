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

		void SetBorderLeft(float borderLeft);
		void SetBorderRight(float borderRight);
		void SetBorderTop(float borderTop);
		void SetBorderBottom(float borderBottom);

		void  SetFlex(float flex);
		virtual void  SetDirty(bool flag);

		float  GetXAbsolute();
		float  GetYAbsolute();
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

		float GetBorderLeft();
		float GetBorderRight();
		float GetBorderTop();
		float GetBorderBottom();

		float  GetFlex();
		bool  GetDirty();

		virtual void SetWidth(float width,bool isPercent = false);
		virtual void SetHeight(float height,bool isPercent = false);
		virtual void SetWidthReal(float widthReal);
		virtual void SetHeightReal(float heightReal);
		virtual float GetWidth();
		virtual float GetHeight();
		virtual float GetWidthReal();
		virtual float GetHeightReal();

		std::vector<std::shared_ptr<Element>> Children;

		
	private:
		void caculateAlignHorizontal();
		void caculateAlignVertical();
		float caculateWidthReal(Layout* element);
		float caculateHeightReal(Layout* element);
		float width = 0.f;
		float height = 0.f;
		float widthReal = 0.f;
		float heightReal = 0.f;
		float xAbsolute = 0.f;
		float yAbsolute = 0.f;
		float marginLeft = 0.f;
		float marginRight = 0.f;
		float marginTop = 0.f;
		float marginBottom = 0.f;
		float paddingLeft = 0.f;
		float paddingRight = 0.f;
		float paddingTop = 0.f;
		float paddingBottom = 0.f;
		float borderLeft = 0.f;
		float borderRight = 0.f;
		float borderTop = 0.f;
		float borderBottom = 0.f;
		bool isWidthPercent = false;
		bool isHeightPercent = false;
		FlexDirection flexDirection = FlexDirection::Row;
		Align alignVertical = Align::Start;
		Align alignHorizontal = Align::Start;
		float flex = 0.f;
		bool isDirty = true;
	};
}


