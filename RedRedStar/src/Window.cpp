#include "../include/RRS/App.h"
#include "../include/RRS/Window.h"
#include "../include/RRS/Element.h"

namespace RRS 
{
	Window::Window()
	{
		
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
		SetDirty(false); //第一次不需要重绘，因为操作系统会自动重绘
		App::Get()->AddWindow(this);
		paintLoopThreadResult = std::async(&Window::paintLoopThread, this);
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
			DestroyWindow(Hwnd);
			disposeSurfaceResource();
			Hwnd = nullptr;
			paintLoopThreadResult.wait();
			App::Get()->RemoveWindow(this);
			OnClosed();
			EmitEvent(EventType::WindowClosed);
		}
	}
	void Window::paintLoopThread()
	{
		while (Hwnd)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(18)); //60帧左右，也不一定每帧都刷新
			if (GetDirty())
			{
				InvalidateRect(Hwnd, nullptr, false);
				SetDirty(false);
			}
		}
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