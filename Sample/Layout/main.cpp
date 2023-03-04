#include <Windows.h>
#include <RRS/App.h>
#include <RRS/CommonType.h>
#include <RRS/Window.h>
#include <RRS/Button.h>

using namespace RRS;
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nCmdShow)
{
	App::Init(hInstance);
	std::wstring hello = L"Hello  ¿ΩÁ£°";
	auto win = std::make_unique<Window>();
	win->SetTitle(hello);
	win->SetFlexDirection(FlexDirection::Row);
	
	win->AddEventListener(EventType::Loaded, [&win](EventListener* arg) {
		auto panel = std::make_shared<Panel>();
		panel->SetWidth(100);
		win->AddChildElement(panel);
		win->Show();
	});
	win->AddEventListener(EventType::WindowClosed, [](EventListener* arg) {
		App::Quit();
	});
	win->Load();
	return App::Exec();
}