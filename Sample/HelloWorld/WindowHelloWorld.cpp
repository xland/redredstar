#include "WindowHelloWorld.h"
#include <RRS/App.h>
#include <RRS/Panel.h>
#include <RRS/Layout.h>
#include <RRS/Color.h>
#include <RRS/Label.h>
using namespace RRS;
WindowHelloWorld::WindowHelloWorld()
{
	Title = L"Hello World";
	SetFlexDirection(FlexDirection::Column);
	SetJustifyContent(JustifyContent::Center);
}
void WindowHelloWorld::OnLoad() 
{
	auto panel = std::make_shared<Panel>();
	panel->SetSize(380, 120);
	panel->SetAlignSelf(LayoutAlign::Center);
	panel->SetBackgroundColor( GetColor(14, 99, 156));
	panel->SetFlexDirection(FlexDirection::Column);
	panel->SetJustifyContent(JustifyContent::Center);

	auto label = std::make_shared<Label>("Hello World");
	label->SetAlignSelf(LayoutAlign::Center);
	panel->AddElement(label);

	AddElement(panel);
	Show();
}
void WindowHelloWorld::OnClosed()
{
	RRS::App::Quit();
}