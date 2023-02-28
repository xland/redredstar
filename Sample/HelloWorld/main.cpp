#include <Windows.h>
#include <memory>
#include <RRS/App.h>
#include "WindowHelloWorld.h"
#include <RRS/EventType.h>
#include <RRS/Window.h>
#include <RRS/Button.h>

using namespace RRS;
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nCmdShow) 
{
	App::Init(hInstance);
	auto win = std::make_unique<Window>();
	win->Title = L"Hello World";
	win->SetFlexDirection(FlexDirection::Column);
	win->SetJustifyContent(JustifyContent::Center);
	win->AddEventListener(EventType::Loaded, [&win](EventListener* arg) {
		auto btn = std::make_shared<Button>();
		btn->SetAlignSelf(LayoutAlign::Center);
		win->AddElement(btn);
		win->Show();
	});
	win->AddEventListener(EventType::WindowClosed, [](EventListener* /* event emitter pointer */) {
		App::Quit();
	});
	win->Load();

	//If you want to implement your own window class to gain more control, read the following code.
	//auto win = std::make_unique<WindowHelloWorld>();
	//win->Load();

	return App::Exec();
}