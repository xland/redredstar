#include "../include/RRS/App.h"
#include <ShellScalingAPI.h>
#include <winuser.h>
namespace RRS {
	App::App(HINSTANCE hInstance):HInstance{hInstance}
	{
	}
	App::~App()
	{
		delete app;
	}
	void App::Init(HINSTANCE hInstance) 
	{
		SetProcessDpiAwareness(PROCESS_SYSTEM_DPI_AWARE);
		app = new App(hInstance);
	}
	int App::Exec() {
		MSG msg = { };
		while (GetMessage(&msg, NULL, 0, 0) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		return (int)msg.wParam;
	}
	App* App::Get() {
		return app;
	}
	void App::Quit() {
		PostQuitMessage(0);
	}
	void App::RemoveWindow(Window* window)
	{
		for (int i = 0; i < Windows.size(); i++) {
			if (Windows.at(i) == window) {
				Windows.erase(Windows.begin() + i);
				break;
			}
		}
	}
}