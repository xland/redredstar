#include "WindowHelloWorld.h"
#include <RRS/App.h>
#include <RRS/Element.h>
WindowHelloWorld::WindowHelloWorld()
{
	title = L"Hello World";
}
void WindowHelloWorld::OnLoad() 
{
	auto ele = new RRS::Element();
	AddElement(ele);
	Show();
}
void WindowHelloWorld::OnClosed()
{
	RRS::App::Quit();
}