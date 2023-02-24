#include "WindowHelloWorld.h"
#include <RRS/App.h>
#include <RRS/Panel.h>
#include <RRS/Layout.h>
#include <RRS/Color.h>
#include <RRS/Label.h>
WindowHelloWorld::WindowHelloWorld()
{
	title = L"Hello World";
	Layout->SetFlexDirection(RRS::FlexDirection::Column);
	Layout->SetJustifyContent(RRS::JustifyContent::Center);
}
void WindowHelloWorld::OnLoad() 
{
	auto panel = new RRS::Panel();
	panel->Layout->SetSize(380, 120);
	panel->Layout->SetFlexDirection(RRS::FlexDirection::Column);
	panel->Layout->SetJustifyContent(RRS::JustifyContent::Center);
	panel->Layout->SetAlignSelf(RRS::LayoutAlign::Center);
	panel->BackgroundColor = RRS::GetColor(14, 99, 156);

	auto label = new RRS::Label("Hello World");
	label->Layout->SetAlignSelf(RRS::LayoutAlign::Center);
	panel->AddElement(label);

	AddElement(panel);
	Show();
}
void WindowHelloWorld::OnClosed()
{
	RRS::App::Quit();
}