#include "../include/RRS/App.h"
namespace RRS {
	App::App(HINSTANCE hInstance):HInstance{hInstance}
	{
	}
	App::~App()
	{
		delete app;
	}
	void App::Init(HINSTANCE hInstance) {
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
}