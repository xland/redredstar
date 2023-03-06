#include "../include/RRS/Layout.h"
#include "../include/RRS/Element.h"
#include <vector>
#include <memory>

namespace RRS {
	float  Layout::caculateWidthReal(Layout* element)
	{
		if (element->isWidthPercent) {
			auto real = GetWidthReal() * element->GetWidth() / 100;
			element->SetWidthReal(real);
		}
		return element->GetWidthReal();
	}
	float  Layout::caculateHeightReal(Layout* element)
	{
		if (element->isHeightPercent) {
			auto real = GetHeightReal() * element->GetHeight() / 100;
			element->SetHeightReal(real);
		}
		return element->GetHeightReal();
	}
	void Layout::caculateAlignHorizontal()
	{
		auto alignHorizontal = GetAlignHorizontal();
		switch (alignHorizontal)
		{
		case RRS::Align::Start:
		{
			auto x = GetXAbsolute() + GetBorderLeft() + GetPaddingLeft();
			for (size_t i = 0; i < Children.size(); i++)
			{
				auto& element = Children[i];
				x += element->GetMarginLeft();
				element->SetXAbsolute(x);
				x += caculateWidthReal(element.get());
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
				childrenWidth += caculateWidthReal(element.get());
				childrenWidth += element->GetMarginRight();
			}
			float boxWidth = GetWidthReal() - GetBorderLeft() - GetPaddingLeft() - GetBorderRight() - GetPaddingRight();
			float x = (boxWidth - childrenWidth) / 2 + GetXAbsolute();
			for (size_t i = 0; i < Children.size(); i++)
			{
				auto& element = Children[i];
				x += element->GetMarginLeft();
				element->SetXAbsolute(x);
				x += element->GetWidthReal();
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
				childrenWidth += caculateWidthReal(element.get());
				childrenWidth += element->GetMarginRight();
			}
			float boxWidth = GetWidthReal() - GetBorderLeft() - GetPaddingLeft() -  GetPaddingRight() - GetBorderRight();
			float x = boxWidth - childrenWidth + GetXAbsolute();
			for (size_t i = 0; i < Children.size(); i++)
			{
				auto& element = Children[i];
				x += element->GetMarginLeft();
				element->SetXAbsolute(x);
				x += element->GetWidthReal();
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
			auto y = GetYAbsolute() + GetPaddingTop() + GetBorderTop();
			for (size_t i = 0; i < Children.size(); i++)
			{
				auto& element = Children[i];
				y += element->GetMarginTop();
				element->SetXAbsolute(y);
				y += caculateHeightReal(element.get());
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
				childrenHeight += caculateHeightReal(element.get());
				childrenHeight += element->GetMarginBottom();
			}
			float boxHeight = GetHeightReal() - GetBorderTop() - GetPaddingTop() - GetPaddingBottom() - GetBorderBottom();
			float y = (boxHeight - childrenHeight) / 2 + GetYAbsolute();
			for (size_t i = 0; i < Children.size(); i++)
			{
				auto& element = Children[i];
				y += element->GetMarginTop();
				element->SetYAbsolute(y);
				y += element->GetHeightReal();
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
				childrenHeight += caculateHeightReal(element.get());
				childrenHeight += element->GetMarginBottom();
			}
			float boxHeight = GetHeightReal() - GetBorderTop() - GetPaddingTop() - GetPaddingBottom() - GetBorderBottom();
			float y = boxHeight - childrenHeight + GetYAbsolute();
			for (size_t i = 0; i < Children.size(); i++)
			{
				auto& element = Children[i];
				y += element->GetMarginTop();
				element->SetYAbsolute(y);
				y += element->GetHeightReal();
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

	void Layout::SetWidth(float width, bool isPercent)
	{
		this->width = width;
		isWidthPercent = isPercent;
		if (!isPercent) this->widthReal = width;
		SetDirty(true);
	}
	void Layout::SetHeight(float height, bool isPercent)
	{
		this->height = height;
		isHeightPercent = isPercent;
		if (!isPercent) this->heightReal = height;
		SetDirty(true);
	}

	float Layout::GetWidth() {
		return width;
	}
	float Layout::GetHeight() {
		return height;
	}
	float Layout::GetWidthReal()
	{
		return widthReal;
	}
	float Layout::GetHeightReal()
	{
		return heightReal;
	}
	void Layout::SetWidthReal(float widthReal)
	{
		this->widthReal = widthReal;
	}
	void Layout::SetHeightReal(float heightReal)
	{
		this->heightReal = heightReal;
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

	void Layout::SetBorderLeft(float borderLeft, Color color){
		this->borderLeft = borderLeft;
		borderLeftColor = color;
		SetDirty(true);
	}
	void Layout::SetBorderRight(float borderRight, Color color){
		this->borderRight = borderRight;
		borderRightColor = color;
		SetDirty(true);
	}
	void Layout::SetBorderTop(float borderTop, Color color){
		this->borderTop = borderTop;
		borderTopColor = color;
		SetDirty(true);
	}
	void Layout::SetBorderBottom(float borderBottom, Color color){
		this->borderBottom = borderBottom;
		borderBottomColor = color;
		SetDirty(true);
	}


	Color Layout::GetBorderLeftColor() { 
		return borderLeftColor; 
	}
	Color Layout::GetBorderRightColor() { return borderRightColor; }
	Color Layout::GetBorderTopColor() { return borderTopColor ; }
	Color Layout::GetBorderBottomColor() { return borderBottomColor; }

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

	float Layout::GetBorderLeft(){
		return borderLeft;
	}
	float Layout::GetBorderRight(){
		return borderRight;
	}
	float Layout::GetBorderTop(){
		return borderTop;
	}
	float Layout::GetBorderBottom(){
		return borderBottom;
	}

	float Layout::GetFlex(){
		return flex;
	}

	bool Layout::GetDirty()
	{
		return isDirty;
	}

	
}