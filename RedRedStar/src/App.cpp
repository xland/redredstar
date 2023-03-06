#include "../include/RRS/App.h"
#include "../include/RRS/Window.h"
#include <ShellScalingAPI.h>
#include <winuser.h>
#include <thread>
#include <chrono>
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
	void App::OnAllWindowClosed(std::function<void()>&& cb)
	{
		app->onAllWindowClosed = cb;
	}
	void App::AddWindow(Window* window)
	{
		windows.push_back(window);		
	}
	void App::RemoveWindow(Window* window)
	{
		for (int i = 0; i < windows.size(); i++) {
			if (windows[i] == window) {
				windows.erase(windows.begin() + i);
				break;
			}
		}
		if (windows.size() < 1 && onAllWindowClosed) {
			onAllWindowClosed();
		}
	}
}