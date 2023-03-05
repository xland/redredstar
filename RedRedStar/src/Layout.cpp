#include "../include/RRS/Layout.h"
#include "../include/RRS/Element.h"
#include <vector>
#include <memory>

namespace RRS {
	void Layout::CaculateLayout()
	{
		auto alignHorizontal = GetAlignHorizontal();
		switch (alignHorizontal)
		{
		case RRS::Align::Start:
			{
			for (const auto& element : Children)
			{
				 
			}
			}
			break;
		case RRS::Align::Center:
			break;
		case RRS::Align::End:
			break;
		default:
			break;
		}
		//if (GetAlignHorizontal() == Align::Center) {
		//	auto x = (parent->GetWidth() - width) / 2 + parent->GetXAbsolute();
		//	SetXAbsolute(x);
		//}
		//if (GetAlignVertical() == Align::Center) {
		//	auto y = (parent->GetHeight() - height) / 2 + parent->GetYAbsolute();
		//	SetYAbsolute(y);
		//}
	}
	void Layout::SetXAbsolute(float xAbsolute)
	{ 
		this->xAbsolute = xAbsolute;
		SetDirty(true);
	}
	void Layout::SetYAbsolute(float yAbsolute)
	{ 
		this->yAbsolute = yAbsolute;
		SetDirty(true);
	}
	void Layout::SetWidthPercent(float widthPercent)
	{ 
		this->widthPercent = widthPercent;
		SetDirty(true);
	}
	void Layout::SetHeightPercent(float heightPercent)
	{ 
		this->heightPercent = heightPercent;
		SetDirty(true);
	}
	void Layout::SetflexDirection(FlexDirection flexDirection)
	{ 
		this->flexDirection = flexDirection;
		SetDirty(true);
	}
	void Layout::SetAlignVertical(Align alignVertical)
	{ 
		this->alignVertical = alignVertical;
		SetDirty(true);
	}
	void Layout::SetAlignHorizontal(Align alignHorizontal)
	{ 
		this->alignHorizontal = alignHorizontal;
		SetDirty(true);
	}
	void Layout::SetFlex(float flex)
	{ 
		this->flex = flex;
		SetDirty(true);
	}
	void Layout::SetDirty(bool flag)
	{
		isDirty = flag;	
	}

	float Layout::GetXAbsolute(){ 
		return xAbsolute;
	}
	float Layout::GetYAbsolute(){ 
		return yAbsolute;
	}
	float Layout::GetWidthPercent(){ 
		return widthPercent;
	}
	float Layout::GetHeightPercent(){ 
		return heightPercent;
	}
	FlexDirection Layout::GetflexDirection(){ 
		return flexDirection;
	}
	Align Layout::GetAlignVertical(){ 
		return alignVertical;
	}
	Align Layout::GetAlignHorizontal(){ 
		return alignHorizontal;
	}
	float Layout::GetFlex(){
		return flex;
	}

	bool Layout::GetDirty()
	{
		return isDirty;
	}

	
}