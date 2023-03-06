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

	void App::paintLoopThread(Window* window)
	{
		while (window->Hwnd)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(18));
			if (window->Hwnd && window->GetDirty())
			{
				InvalidateRect(window->Hwnd, nullptr, false);
				window->SetDirty(false);
			}
		}
		delete window;
	}
	void App::AddWindow(Window* window)
	{
		Windows.push_back(window);
		std::thread t(&App::paintLoopThread, app,window);
		t.detach();
	}
	void App::RemoveWindow(Window* window)
	{
		for (int i = 0; i < Windows.size(); i++) {
			if (Windows[i] == window) {
				Windows.erase(Windows.begin() + i);
				break;
			}
		}
		if (Windows.size() < 1 && onAllWindowClosed) {
			std::this_thread::sleep_for(std::chrono::milliseconds(20)); //等待资源释放完毕
			onAllWindowClosed();
		}
	}
}