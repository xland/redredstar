#include <Windows.h>
#include <memory>
#include <RRS/App.h>
#include "WindowHelloWorld.h"
#include <RRS/EventType.h>
#include <RRS/Window.h>
#include <RRS/Panel.h>
#include <RRS/Label.h>

using namespace RRS;
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nCmdShow) 
{
	App::Init(hInstance);
	auto win = std::make_unique<Window>();
	win->Title = L"Hello World";
	win->SetFlexDirection(FlexDirection::Column);
	win->SetJustifyContent(JustifyContent::Center);
	win->AddEventListener(EventType::Loaded, [&win](EventListener* arg) {
		auto panel = std::make_shared<Panel>();
		panel->SetSize(380, 120);
		panel->SetAlignSelf(LayoutAlign::Center);
		panel->BackgroundColor = GetColor(28, 88, 156);
		panel->SetFlexDirection(FlexDirection::Column);
		panel->SetJustifyContent(JustifyContent::Center);

		panel->AddEventListener(EventType::MouseOver, [&panel](EventListener* arg) {
			panel->BackgroundColor = GetColor(88, 28, 156);
		});
		panel->AddEventListener(EventType::MouseOut, [&panel](EventListener* arg) {
			panel->BackgroundColor = GetColor(28, 88, 156);
		});

		auto label = std::make_shared<Label>("Hello World");
		label->SetAlignSelf(LayoutAlign::Center);
		panel->AddElement(label);

		win->AddElement(panel);
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