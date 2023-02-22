#include "WindowHelloWorld.h"
#include <RRS/App.h>
WindowHelloWorld::WindowHelloWorld()
{
	title = L"Hello World";
}
void WindowHelloWorld::OnLoad() 
{
	Show();
}
void WindowHelloWorld::OnClosed()
{
	RRS::App::Quit();
}