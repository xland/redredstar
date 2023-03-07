#include "../include/RRS/Layout.h"
#include "../include/RRS/Element.h"
#include <vector>
#include <memory>

namespace RRS {

	void Layout::CaculateRealSize(Layout* parent)
	{
		if (isWidthPercent) {
			auto w = parent->GetWidthReal() 
				- parent->GetBorderLeft()  - parent->GetPaddingLeft() 
				- parent->GetPaddingRight() - parent->GetBorderRight();
			widthReal = w * width / 100;
		}
		if (isHeightPercent) {
			auto h = parent->GetHeightReal()
				- parent->GetBorderTop() - parent->GetPaddingTop()
				- parent->GetPaddingBottom() - parent->GetBorderBottom();
			heightReal = h * height / 100;
		}
	}
	void Layout::caculateRowVertical(Element* element) {
		if (alignVertical == Align::Start) {
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
		else if (alignVertical == Align::Flex || alignVertical == Align::SpaceBetween || alignVertical == Align::SpaceAround) {
			auto itemY = yAbsolute + borderTop + paddingTop + element->GetMarginTop();
			element->SetYAbsolute(itemY);
		}
	}
	void Layout::caculateColumnHorizontal(Element* element) 
	{
		if (alignHorizontal == Align::Start) {
			auto itemX = xAbsolute + borderLeft + paddingLeft + element->GetMarginLeft();
			element->SetXAbsolute(itemX);
		}
		else if (alignHorizontal == Align::Center) {
			auto itemX = xAbsolute + widthReal / 2 - element->GetWidthReal() / 2 + element->GetMarginLeft();
			element->SetXAbsolute(itemX);
		}
		else if (alignHorizontal == Align::End) {
			auto itemX = xAbsolute + widthReal - element->GetWidthReal() - element->GetMarginRight();
			element->SetXAbsolute(itemX);
		}
		else if (alignHorizontal == Align::Flex || alignVertical == Align::SpaceBetween || alignVertical == Align::SpaceAround) {
			auto itemY = yAbsolute + borderTop + paddingTop + element->GetMarginTop();
			element->SetYAbsolute(itemY);
		}
	}
	void Layout::CaculateLayout()
	{
		if (layoutDirection == LayoutDirection::Row) {
			if (alignHorizontal == Align::Start) {
				auto x = xAbsolute + borderLeft + paddingLeft;
				for (auto& element : Children)
				{
					element->CaculateRealSize(this);
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
					element->CaculateRealSize(this);
					childrenWidth += element->GetMarginLeft()+element->GetWidthReal()+element->GetMarginRight();
				}
				float x = xAbsolute+ widthReal/2-childrenWidth/2;
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
				float x = xAbsolute + widthReal - paddingRight - borderRight;
				for (int i = Children.size() - 1; i > -1; i--)
				{
					auto& element = Children[i];
					element->CaculateRealSize(this);
					x -= element->GetMarginRight();
					x -= element->GetWidthReal();
					element->SetXAbsolute(x);
					caculateRowVertical(element.get());
					x -= element->GetMarginLeft();
				}
			}
			else if (alignHorizontal == Align::Flex)
			{
				auto x = xAbsolute + borderLeft + paddingLeft;
				auto w = widthReal - borderLeft - paddingLeft - borderRight - paddingRight;
				auto flexSum = 0;
				for (auto& element:Children)
				{
					element->CaculateRealSize(this);
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
		else if (layoutDirection == LayoutDirection::Column)
		{
			if (alignVertical == Align::Start)
			{
				auto y = yAbsolute + paddingTop + borderTop;
				for (auto& element : Children)
				{
					element->CaculateRealSize(this);
					y += element->GetMarginTop();
					element->SetYAbsolute(y);
					caculateColumnHorizontal(element.get());
					y += element->GetHeightReal() + element->GetMarginBottom();
				}
			}
			else if (alignVertical == Align::Center)
			{
				float childrenHeight = 0.f;
				for (const auto& element : Children)
				{
					element->CaculateRealSize(this);
					childrenHeight += element->GetMarginTop() + element->GetHeightReal() + element->GetMarginBottom();
				}
				float y = yAbsolute  + heightReal/2 - childrenHeight/2;
				for (auto& element : Children)
				{
					y += element->GetMarginTop();
					element->SetYAbsolute(y);
					caculateColumnHorizontal(element.get());
					y += element->GetHeightReal()+ element->GetMarginBottom();
				}
			}
			else if (alignVertical == Align::End)
			{
				float y = yAbsolute + heightReal - paddingBottom - borderBottom;
				for (int i = Children.size()-1; i >-1; i--)
				{
					auto& element = Children[i];
					element->CaculateRealSize(this);
					y -= element->GetMarginBottom();
					y -= element->GetHeightReal();
					element->SetYAbsolute(y);
					caculateColumnHorizontal(element.get());
					y -= element->GetMarginTop();
				}
			}
			else if (alignVertical == Align::Flex)
			{
				auto y = yAbsolute + borderTop + paddingTop;
				auto h = heightReal - borderTop - paddingTop - borderBottom - paddingBottom;
				auto flexSum = 0;
				for (auto& element : Children)
				{
					element->CaculateRealSize(this);
					if (element->GetFlex() == 0.f) {
						h = h - element->GetMarginTop() - element->GetHeightReal() - element->GetMarginBottom();
					}
					else
					{
						flexSum += element->GetFlex();
					}
				}
				for (auto& element : Children)
				{
					y = y + element->GetMarginTop();
					if (element->GetFlex() != 0.f)
					{
						element->SetHeightReal(h / flexSum * element->GetFlex());
					}
					element->SetYAbsolute(y);
					caculateColumnHorizontal(element.get());
					y += element->GetHeightReal() + element->GetMarginBottom();
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