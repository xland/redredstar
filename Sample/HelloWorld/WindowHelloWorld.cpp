#include "WindowHelloWorld.h"
#include <RRS/App.h>
#include <RRS/Element.h>
#include <RRS/Layout.h>
#include <RRS/Color.h>
WindowHelloWorld::WindowHelloWorld()
{
	title = L"Hello World";
	Layout->SetFlexDirection(RRS::FlexDirection::Column);
	Layout->SetJustifyContent(RRS::JustifyContent::Center);
}
void WindowHelloWorld::OnLoad() 
{
	auto ele = new RRS::Element();
	ele->Layout->SetSize(200, 200);
	ele->Layout->SetAlignSelf(RRS::LayoutAlign::Center);
	ele->BackgroundColor = RRS::GetColor(100, 60, 80, 255);
	AddElement(ele);
	Show();
}
void WindowHelloWorld::OnClosed()
{
	RRS::App::Quit();
}