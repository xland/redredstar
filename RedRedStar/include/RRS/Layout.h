#pragma once
#include "LayoutEnum.h"

namespace RRS {
	class Layout
	{
	public:
		Layout() = default;
		~Layout() = default;
		void  SetXAbsolute(float xAbsolute);
		void  SetYAbsolute(float yAbsolute);
		void  SetWidthPercent(float widthPercent);
		void  SetHeightPercent(float heightPercent);
		void  SetflexDirection(FlexDirection flexDirection);
		void  SetAlignVertical(Align alignVertical);
		void  SetAlignHorizontal(Align alignHorizontal);
		void  SetFlex(float flex);
		virtual void  SetDirty(bool flag);

		float  GetXAbsolute();
		float  GetYAbsolute();
		float  GetWidthPercent();
		float  GetHeightPercent();
		FlexDirection  GetflexDirection();
		Align  GetAlignVertical();
		Align  GetAlignHorizontal();
		float  GetFlex();
		bool  GetDirty();

		virtual void SetWidth(float width) = 0;
		virtual void SetHeight(float height) = 0;
		virtual float GetWidth() = 0;
		virtual float GetHeight() = 0;
	private:
		float xAbsolute = 0.f;
		float yAbsolute = 0.f;
		float widthPercent = 0.f;
		float heightPercent = 0.f;
		bool useWidthPercent = false;
		bool useHeightPercent = false;
		FlexDirection flexDirection = FlexDirection::Row;
		Align alignVertical = Align::Start;
		Align alignHorizontal = Align::Start;
		float flex = 0.f;
		bool isDirty = true;
	};
}


