#include <Windows.h>
#include <RRS/App.h>
#include <RRS/CommonType.h>
#include <RRS/Window.h>
#include <RRS/Button.h>
#include <functional>

using namespace RRS;
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nCmdShow)
{
	App::Init(hInstance);
	auto win = std::make_unique<Window>();
	win->WindowTitle = L"Hello ÊÀ½ç£¡";
	win->AddEventListener(EventType::Loaded, [&win]() {
		auto panel = std::make_shared<Element>();
		panel->SetBackgroundColor(GetColor(230, 231, 232));
		panel->SetBorderRight(2, GetColor(204, 213, 240));
		panel->SetWidth(300);
		panel->SetHeight(100,true);
		win->AddChild(panel);
		win->Show();
	});
	win->AddEventListener(EventType::WindowClosed, [&win]() {
		win.reset();
		App::Quit();
	});
	win->Load();
	return App::Exec();
}