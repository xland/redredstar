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
		constexpr Color selectedColor = GetColor(168, 188, 168);
		auto menu = std::make_shared<Element>();
		menu->SetLayoutDirection(LayoutDirection::Column);
		menu->SetBackgroundColor(GetColor(247, 248, 249));
		menu->SetBorderRight(1, borderColor);
		menu->SetBorderTop(1, borderColor);
		menu->SetWidth(300);
		menu->SetHeight(100,true);

		auto menuItem1 = std::make_shared<ButtonSelectable>(L"Row Layout");
		menuItem1->SetAlignHorizontal(Align::Start);
		menuItem1->SetPaddingLeft(26);
		menuItem1->SetWidth(100, true);
		menuItem1->SetBorderBottom(1, borderColor);
		menu->AddChild(menuItem1);		

		auto menuItem2 = std::make_shared<ButtonSelectable>(L"Row Flex Layout");
		menuItem2->SetAlignHorizontal(Align::Start);
		menuItem2->SetPaddingLeft(26);
		menuItem2->SetWidth(100, true);
		menuItem2->SetBorderBottom(1, borderColor);
		menu->AddChild(menuItem2);

		auto menuItem3 = std::make_shared<ButtonSelectable>(L"Column Layout");
		menuItem3->SetAlignHorizontal(Align::Start);
		menuItem3->SetPaddingLeft(26);
		menuItem3->SetWidth(100, true);
		menuItem3->SetBorderBottom(1, borderColor);
		menu->AddChild(menuItem3);

		auto menuItem4 = std::make_shared<ButtonSelectable>(L"Column Flex Layout");
		menuItem4->SetAlignHorizontal(Align::Start);
		menuItem4->SetPaddingLeft(26);
		menuItem4->SetWidth(100, true);
		menuItem4->SetBorderBottom(1, borderColor);
		menu->AddChild(menuItem4);

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