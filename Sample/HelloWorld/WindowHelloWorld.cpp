#include "WindowHelloWorld.h"

WindowHelloWorld::WindowHelloWorld()
{

}
void WindowHelloWorld::OnLoad() 
{
	Show();
}
bool WindowHelloWorld::IsMouseInCaptionArea(int x, int y)
{
	return true;
}