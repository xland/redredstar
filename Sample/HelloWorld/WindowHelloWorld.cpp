#include "WindowHelloWorld.h"
#include <RRS/App.h>
#include <RRS/Element.h>
#include <RRS/Layout.h>
#include <RRS/Color.h>
WindowHelloWorld::WindowHelloWorld()
{
	title = L"Hello World";
	BackgroundColor = RRS::GetColor(30, 30, 30);
	Layout->SetFlexDirection(RRS::FlexDirection::Column);
	Layout->SetJustifyContent(RRS::JustifyContent::Center);
}
void WindowHelloWorld::OnLoad() 
{
	auto ele = new RRS::Element();
	ele->Layout->SetSize(380, 120);
	ele->Layout->SetFlexDirection(RRS::FlexDirection::Column);
	ele->Layout->SetJustifyContent(RRS::JustifyContent::Center);
	ele->Layout->SetAlignSelf(RRS::LayoutAlign::Center);
	ele->BackgroundColor = RRS::GetColor(14, 99, 156);



	AddElement(ele);
	Show();
}
void WindowHelloWorld::OnClosed()
{
	RRS::App::Quit();
}