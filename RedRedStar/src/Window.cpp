#include "../include/RRS/App.h"
#include "../include/RRS/Window.h"
#include "../include/RRS/Element.h"
#include <thread>
#include <chrono>

namespace RRS 
{
	Window::Window()
	{
		App::Get()->Windows.push_back(this);
	}
	Window::~Window()
	{
	}
	bool Window::Load() 
	{
		auto flag = createNativeWindow();
		if (!flag) return flag; //todo error
		initSurface();
		OnLoad();
		EmitEvent(EventType::Loaded);
		SetDirty(false);
		std::thread t(&Window::paintLoopThread, this);
		t.detach();
		return true;
	}
	void Window::AddChild(std::shared_ptr<Element> child)
	{
		child->OwnerWindow = this;
		Children.push_back(child);
		//todo 不应该用递归，应该改成简单的循环，防止栈溢出
		std::function<void(Element* ele)> func = [this,&func](Element* ele) {
			ele->OwnerWindow = this;
			for (auto& c : ele->Children)
			{
				func(c.get());
			}
		};
		for (auto& c : child->Children)
		{
			func(c.get());
		}
	}
	void Window::Close()
	{
		auto flag = OnClose();
		if (flag) {
			App::Get()->RemoveWindow(this);
			disposeSurfaceResource();
			DestroyWindow(Hwnd);
			OnClosed();
			EmitEvent(EventType::WindowClosed);
		}
	}
	void Window::paintLoopThread()
	{
		while (true)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(18));
			if (GetDirty())
			{
				InvalidateRect(Hwnd, nullptr, false);
				SetDirty(false);
			}
		}
	}
	void Window::SetWidth(float width)
	{
	}
	void Window::SetHeight(float height)
	{
	}

	float Window::GetWidth() {
		return WidthClient;
	}
	float Window::GetHeight() {
		return HeightClient;
	}
	void Window::Show() 
	{
		ShowWindow(Hwnd, SW_SHOW);
		EmitEvent(EventType::Show);
	}
	void Window::Hide() 
	{
		//todo
		EmitEvent(EventType::Hide);
	}
}