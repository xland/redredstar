#include <Windows.h>
#include <memory>
#include <RRS/App.h>
#include "WindowHelloWorld.h"

using namespace RRS;
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nCmdShow) 
{
	App::Init(hInstance);
	auto win = std::make_unique<WindowHelloWorld>();
	win->Load();
	return App::Exec();
}