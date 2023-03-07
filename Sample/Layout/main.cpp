#include <Windows.h>
#include <RRS/App.h>
#include <RRS/CommonType.h>
#include <RRS/Window.h>
#include <RRS/ButtonSelectable.h>
#include <functional>

using namespace RRS;
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nCmdShow)
{
	App::Init(hInstance);
	auto win = std::make_unique<Window>();
	win->WindowTitle = L"RedRedStar Layout";
	win->SetAlignHorizontal(Align::Flex);
	win->AddEventListener(EventType::Loaded, [&win]() {
		constexpr Color borderColor = GetColor(188, 188, 168);
		auto menu = std::make_shared<Element>();
		menu->SetLayoutDirection(LayoutDirection::Column);
		menu->SetBackgroundColor(GetColor(247, 248, 249));
		menu->SetBorderRight(1, borderColor);
		menu->SetBorderTop(1, borderColor);
		menu->SetWidth(300);
		menu->SetHeight(100,true);

		auto menuItem = std::make_shared<ButtonSelectable>(L"Row Layout");
		menuItem->SetWidth(100, true);
		menu->AddChild(menuItem);

		auto body = std::make_shared<Element>();
		body->SetBorderTop(1, borderColor);
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