#include <Windows.h>
#include <RRS/App.h>
#include <RRS/CommonType.h>
#include <RRS/Window.h>
#include <RRS/Button.h>

using namespace RRS;
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nCmdShow) 
{
	App::Init(hInstance);
	std::wstring hello = L"Hello 世界";
	auto win = std::make_unique<Window>();
	win->SetAlignHorizontal(Align::Center);
	win->SetAlignVertical(Align::Center);
	win->WindowTitle = hello;
	win->AddEventListener(EventType::Loaded, [&win,&hello]() {		
		auto btn = std::make_shared<Button>(hello);
		btn->AddEventListener(EventType::Click, [&win,&hello]() {
			MessageBox(win->Hwnd, hello.c_str(), L"系统提示", MB_ICONWARNING | MB_OK | MB_DEFBUTTON1);
		});
		win->AddChild(btn);
		win->Show();
	});
	App::OnAllWindowClosed([]() {
		App::Quit();
	});
	win->Load();
	return App::Exec();
}