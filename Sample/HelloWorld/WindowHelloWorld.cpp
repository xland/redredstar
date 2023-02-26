#include "WindowHelloWorld.h"
#include <RRS/App.h>
#include <RRS/Panel.h>
#include <RRS/Layout.h>
#include <RRS/Color.h>
#include <RRS/Label.h>
WindowHelloWorld::WindowHelloWorld()
{
	Title = L"Hello World";
	SetFlexDirection(RRS::FlexDirection::Column);
	SetJustifyContent(RRS::JustifyContent::Center);
}
void WindowHelloWorld::OnLoad() 
{
	auto panel = new RRS::Panel();
	panel->SetSize(380, 120);
	panel->SetAlignSelf(RRS::LayoutAlign::Center);
	panel->BackgroundColor = RRS::GetColor(14, 99, 156);
	panel->SetFlexDirection(RRS::FlexDirection::Column);
	panel->SetJustifyContent(RRS::JustifyContent::Center);

	auto label = new RRS::Label("Hello World");
	label->SetAlignSelf(RRS::LayoutAlign::Center);
	panel->AddElement(label);

	AddElement(panel);
	Show();
}
void WindowHelloWorld::OnClosed()
{
	RRS::App::Quit();
}