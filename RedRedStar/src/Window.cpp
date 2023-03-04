#include "../include/RRS/App.h"
#include "../include/RRS/Window.h"
#include "../include/RRS/Element.h"

namespace RRS 
{
	Window::Window()
	{
		RootElement = new Element();
		RootElement->OwnerWindow = this;
		App::Get()->Windows.push_back(this);
	}
	Window::~Window()
	{
		delete RootElement;
	}
	bool Window::Load() 
	{
		auto flag = createNativeWindow();
		if (!flag) return flag; //todo error
		initSurface();
		OnLoad();
		EmitEvent(EventType::Loaded);
		return true;
	}
	void Window::AddChild(std::shared_ptr<Element> child)
	{
		child->OwnerWindow = this;
		RootElement->AddChild(child);
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