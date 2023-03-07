#include "../include/RRS/Layout.h"
#include "../include/RRS/Element.h"
#include <vector>
#include <memory>

namespace RRS {

	void Layout::CaculateRealSize(float w,float h)
	{
		if (isWidthPercent) {
			widthReal = w * width / 100;
		}
		if (isHeightPercent) {
			heightReal = h * height / 100;
		}
	}

	void Layout::caculateAlignHorizontal()
	{		
		switch (alignHorizontal)
		{
		case RRS::Align::Start:
		{
			auto x = GetXAbsolute() + GetBorderLeft() + GetPaddingLeft();
			auto y = GetYAbsolute() + GetBorderTop() + GetPaddingTop();
			for (size_t i = 0; i < Children.size(); i++)
			{
				auto& element = Children[i];
				//caculateWidthReal(element.get());
				x += element->GetMarginLeft();
				element->SetXAbsolute(x);
				switch (alignVertical)
				{
				default:
					break;
				}
				element->SetYAbsolute(y);
				x += element->GetWidthReal();
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
				//childrenWidth += caculateWidthReal(element.get());
				childrenWidth += element->GetMarginRight();
			}
			float boxWidth = GetWidthReal() - GetBorderLeft() - GetPaddingLeft() - GetBorderRight() - GetPaddingRight();
			float x = (boxWidth - childrenWidth) / 2 + GetXAbsolute();
			for (size_t i = 0; i < Children.size(); i++)
			{
				auto& element = Children[i];
				x += element->GetMarginLeft();
				element->SetXAbsolute(x);
				x += element->GetWidthReal() + element->GetMarginRight();
			}
			break;
		}
		case RRS::Align::End:
		{
			float x = GetXAbsolute() + GetWidthReal() - GetBorderRight() - GetPaddingRight();
			for (int i = Children.size()-1; i > -1; i--)
			{
				auto& element = Children[i];
				//x = x - element->GetMarginRight() - caculateWidthReal(element.get());
				element->SetXAbsolute(x);
				x -= element->GetMarginLeft();
			}
			break;
		}
		case RRS::Align::Flex:
		{
			auto x = GetXAbsolute() + GetBorderLeft() + GetPaddingLeft();
			auto w = GetWidthReal() - GetBorderLeft() - GetPaddingLeft() - GetBorderRight() - GetPaddingRight();
			auto flexSum = 0;
			for (size_t i = 0; i < Children.size(); i++)
			{
				auto& element = Children[i];
				if (element->GetFlex() == 0.f) {
					//w = w - element->GetMarginLeft() - caculateWidthReal(element.get()) - element->GetMarginRight();
				}
				else
				{
					flexSum += element->GetFlex();
				}
			}
			for (size_t i = 0; i < Children.size(); i++)
			{
				auto& element = Children[i];
				x = x + element->GetMarginLeft();
				if (element->GetFlex() != 0.f) 
				{					
					element->SetWidthReal(w / flexSum * element->GetFlex());
				}
				element->SetXAbsolute(x);
				x += element->GetWidthReal() + element->GetMarginRight();
			}
			break;
		}
		case RRS::Align::SpaceAround:
		{
			auto x = GetXAbsolute() + GetWidthReal() - GetBorderRight() + GetPaddingRight();
			auto w = GetWidthReal() - GetBorderLeft() - GetPaddingLeft() - GetBorderRight() - GetPaddingRight();
			auto flexSum = 0;
			for (size_t i = 0; i < Children.size(); i++)
			{
				auto& element = Children[i];
				if (element->GetFlex() == 0.f) {
					//w = w - element->GetMarginLeft() - caculateWidthReal(element.get()) - element->GetMarginRight();
				}
				else
				{
					flexSum += element->GetFlex();
				}
			}
			for (int i = Children.size() - 1; i > -1; i--)
			{
				auto& element = Children[i];
				if (element->GetFlex() != 0.f)
				{
					element->SetWidthReal(w / flexSum * element->GetFlex());
					
				}
				x = x - element->GetMarginRight() - GetWidthReal();
				element->SetXAbsolute(x);
				x -= element->GetMarginLeft();
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
				element->SetYAbsolute(y);
				//y += caculateHeightReal(element.get());
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
				//childrenHeight += caculateHeightReal(element.get());
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
				//childrenHeight += caculateHeightReal(element.get());
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
	void Layout::caculateRowVertical(Element* element) {
		if (alignVertical == Align::Start || alignVertical == Align::Flex || alignVertical == Align::SpaceBetween || alignVertical == Align::SpaceAround) {
			auto itemY = yAbsolute + borderTop + paddingTop + element->GetMarginTop();
			element->SetYAbsolute(itemY);
		}
		else if (alignVertical == Align::Center) {
			auto itemY = yAbsolute + heightReal / 2 - element->GetHeightReal() / 2 + element->GetMarginTop();
			element->SetYAbsolute(itemY);
		}
		else if (alignVertical == Align::End) {
			auto itemY = yAbsolute + heightReal - element->GetHeightReal() - element->GetMarginBottom();
			element->SetYAbsolute(itemY);
		}
	}
	void Layout::CaculateLayout()
	{
		//caculateAlignHorizontal();
		//caculateAlignVertical();
		if (layoutDirection == LayoutDirection::Row) {
			if (alignHorizontal == Align::Start) {
				auto x = xAbsolute + borderLeft + paddingLeft;
				for (auto& element : Children)
				{
					element->CaculateRealSize(widthReal, heightReal);
					x += element->GetMarginLeft();
					element->SetXAbsolute(x);
					caculateRowVertical(element.get());
					x += element->GetWidthReal() + element->GetMarginRight();
				}
			}
			else if(alignHorizontal == Align::Center)
			{
				float childrenWidth = 0.f;
				for (const auto& element : Children)
				{
					element->CaculateRealSize(widthReal, heightReal);
					childrenWidth += element->GetMarginLeft()+element->GetWidthReal()+element->GetMarginRight();
				}
				float boxWidth = widthReal - borderLeft - paddingLeft - borderRight - paddingRight;
				float x = (boxWidth - childrenWidth) / 2 + xAbsolute;
				for (auto& element : Children)
				{
					x += element->GetMarginLeft();
					element->SetXAbsolute(x);
					caculateRowVertical(element.get());
					x += element->GetWidthReal() + element->GetMarginRight();
				}
			}
			else if (alignHorizontal == Align::End)
			{
			}
			else if (alignHorizontal == Align::Flex)
			{
				auto x = xAbsolute + borderLeft + paddingLeft;
				auto w = widthReal - borderLeft - paddingLeft - borderRight - paddingRight;
				auto flexSum = 0;
				for (auto& element:Children)
				{
					element->CaculateRealSize(widthReal, heightReal);
					if (element->GetFlex() == 0.f) {
						w = w - element->GetMarginLeft() - element->GetWidthReal() - element->GetMarginRight();
					}
					else
					{
						flexSum += element->GetFlex();
					}
				}
				for (auto& element : Children)
				{
					x = x + element->GetMarginLeft();
					if (element->GetFlex() != 0.f)
					{
						element->SetWidthReal(w / flexSum * element->GetFlex());
					}
					element->SetXAbsolute(x);
					caculateRowVertical(element.get());
					x += element->GetWidthReal() + element->GetMarginRight();
				}
			}
		}

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
	void Layout::SetLayoutDirection(LayoutDirection layoutDirection)
	{ 
		this->layoutDirection = layoutDirection;
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
	LayoutDirection Layout::GetLayoutDirection(){ 
		return layoutDirection;
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