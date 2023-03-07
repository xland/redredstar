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
	win->WindowTitle = L"RedRedStar Layout";
	win->SetAlignHorizontal(Align::Flex);
	win->AddEventListener(EventType::Loaded, [&win]() {
		auto menu = std::make_shared<Element>();
		menu->SetBackgroundColor(GetColor(247, 248, 249));
		menu->SetBorderRight(1, GetColor(221, 221, 221));
		menu->SetBorderTop(1, GetColor(221, 221, 221));
		menu->SetWidth(300);
		menu->SetHeight(100,true);

		auto body = std::make_shared<Element>();
		body->SetBorderTop(1, GetColor(221, 221, 221));
		body->SetFlex(1.f);
		body->SetHeight(100, true);
		body->SetBackgroundColor(GetColor(229, 229, 229));

		win->AddChild(menu);
		win->AddChild(body);
		win->Show();
	});
	win->AddEventListener(EventType::WindowClosed, [&win]() {
		win.reset();
		App::Quit();
	});
	win->Load();
	return App::Exec();
}