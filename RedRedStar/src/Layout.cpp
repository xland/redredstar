#include "../include/RRS/Layout.h"
#include "../include/RRS/Element.h"
#include <vector>
#include <memory>

namespace RRS {
	void Layout::caculateAlignHorizontal()
	{
		auto alignHorizontal = GetAlignHorizontal();
		switch (alignHorizontal)
		{
		case RRS::Align::Start:
		{
			auto x = GetXAbsolute() + GetPaddingLeft();
			for (size_t i = 0; i < Children.size(); i++)
			{
				auto& element = Children[i];
				x += element->GetMarginLeft();
				element->SetXAbsolute(x);
				x += element->GetWidth();
				x += element->GetMarginRight();
			}
			break;
		}
		case RRS::Align::Center:
		{
			float childrenWidth = 0.f;
			for (const auto& element : Children)
			{
				childrenWidth += element->GetMarginLeft();
				childrenWidth += element->GetWidth();
				childrenWidth += element->GetMarginRight();
			}
			float boxWidth = GetWidth() - GetPaddingLeft() - GetPaddingRight();
			float x = (boxWidth - childrenWidth) / 2 + GetXAbsolute();
			for (size_t i = 0; i < Children.size(); i++)
			{
				auto& element = Children[i];
				x += element->GetMarginLeft();
				element->SetXAbsolute(x);
				x += element->GetWidth();
				x += element->GetMarginRight();
			}
			break;
		}
		case RRS::Align::End:
		{
			float childrenWidth = 0.f;
			for (const auto& element : Children)
			{
				childrenWidth += element->GetMarginLeft();
				childrenWidth += element->GetWidth();
				childrenWidth += element->GetMarginRight();
			}
			float boxWidth = GetWidth() - GetPaddingLeft() - GetPaddingRight();
			float x = boxWidth - childrenWidth + GetXAbsolute();
			for (size_t i = 0; i < Children.size(); i++)
			{
				auto& element = Children[i];
				x += element->GetMarginLeft();
				element->SetXAbsolute(x);
				x += element->GetWidth();
				x += element->GetMarginRight();
			}
			break;
		}
		default:
			break;
		}
	}
	void Layout::caculateAlignVertical()
	{
		auto alignVertical = GetAlignVertical();
		switch (alignVertical)
		{
		case RRS::Align::Start:
		{
			auto y = GetYAbsolute() + GetPaddingTop();
			for (size_t i = 0; i < Children.size(); i++)
			{
				auto& element = Children[i];
				y += element->GetMarginTop();
				element->SetXAbsolute(y);
				y += element->GetHeight();
				y += element->GetMarginBottom();
			}
			break;
		}
		case RRS::Align::Center:
		{
			float childrenHeight = 0.f;
			for (const auto& element : Children)
			{
				childrenHeight += element->GetMarginTop();
				childrenHeight += element->GetHeight();
				childrenHeight += element->GetMarginBottom();
			}
			float boxHeight = GetHeight() - GetPaddingTop() - GetPaddingBottom();
			float y = (boxHeight - childrenHeight) / 2 + GetYAbsolute();
			for (size_t i = 0; i < Children.size(); i++)
			{
				auto& element = Children[i];
				y += element->GetMarginTop();
				element->SetYAbsolute(y);
				y += element->GetHeight();
				y += element->GetMarginBottom();
			}
			break;
		}
		case RRS::Align::End:
		{
			float childrenHeight = 0.f;
			for (const auto& element : Children)
			{
				childrenHeight += element->GetMarginTop();
				childrenHeight += element->GetHeight();
				childrenHeight += element->GetMarginBottom();
			}
			float boxHeight = GetWidth() - GetPaddingTop() - GetPaddingBottom();
			float y = boxHeight - childrenHeight + GetYAbsolute();
			for (size_t i = 0; i < Children.size(); i++)
			{
				auto& element = Children[i];
				y += element->GetMarginTop();
				element->SetYAbsolute(y);
				y += element->GetHeight();
				y += element->GetMarginBottom();
			}
			break;
		}
		default:
			break;
		}
	}
	void Layout::CaculateLayout()
	{
		caculateAlignHorizontal();
		caculateAlignVertical();
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
	
	void Layout::SetMarginLeft(float marginLeft)
	{
		this->marginLeft = marginLeft;
		SetDirty(true);
	}
	void Layout::SetMarginRight(float marginRight){
		this->marginRight = marginRight;
		SetDirty(true);
	}
	void Layout::SetMarginTop(float marginTop){ 
		this->marginTop = marginTop;
		SetDirty(true);
	}
	void Layout::SetMarginBottom(float marginBottom){ 
		this->marginBottom = marginBottom;
		SetDirty(true);
	}
	void Layout::SetPaddingLeft(float paddingLeft){ 
		this->paddingLeft = paddingLeft;
		SetDirty(true);
	}
	void Layout::SetPaddingRight(float paddingRight){	
		this->paddingRight = paddingRight;
		SetDirty(true);
	}
	void Layout::SetPaddingTop(float paddingTop){ 
		this->paddingTop = paddingTop;
		SetDirty(true);
	}
	void Layout::SetPaddingBottom(float paddingBottom){ 
		this->paddingBottom = paddingBottom;
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

	float Layout::GetMarginLeft(){ 
		return marginLeft;
	}
	float Layout::GetMarginRight(){ 
		return marginRight;
	}
	float Layout::GetMarginTop(){
		return marginTop;
	}
	float Layout::GetMarginBottom(){
		return marginBottom;
	}
	float Layout::GetPaddingLeft(){ 
		return paddingLeft;
	}
	float Layout::GetPaddingRight(){ 
		return paddingRight;
	}
	float Layout::GetPaddingTop(){ 
		return paddingTop;
	}
	float Layout::GetPaddingBottom(){ 
		return paddingBottom;
	}

	float Layout::GetFlex(){
		return flex;
	}

	bool Layout::GetDirty()
	{
		return isDirty;
	}

	
}