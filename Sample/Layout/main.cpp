#include <Windows.h>
#include <RRS/App.h>
#include <RRS/CommonType.h>
#include <RRS/Window.h>
#include <RRS/Button.h>
using namespace RRS;
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nCmdShow)
{
	App::Init(hInstance);
	std::wstring hello = L"Hello 世界！";
	auto win = std::make_unique<Window>();
	win->SetTitle(hello);
	win->SetFlexDirection(FlexDirection::Column);
	win->SetJustifyContent(JustifyContent::Center);
	win->AddEventListener(EventType::Loaded, [&win, &hello](EventListener* arg) {
		auto btn = std::make_shared<Button>(hello);
		btn->SetAlignSelf(LayoutAlign::Center);
		btn->AddEventListener(EventType::Click, [&win, &hello](EventListener* arg) {
			MessageBox(win->Hwnd, hello.c_str(), L"系统提示", MB_ICONWARNING | MB_OK | MB_DEFBUTTON1);
			});
		win->AddChildElement(btn);
		win->Show();
		});
	win->AddEventListener(EventType::WindowClosed, [](EventListener* arg) {
		App::Quit();
		});
	win->Load();
	return App::Exec();
}