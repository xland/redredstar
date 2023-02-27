#include "../include/RRS/App.h"
#include "../include/RRS/Window.h"
#include "../include/RRS/Element.h"
#include "../include/RRS/Layout.h"
#include <Yoga.h>

namespace RRS 
{
	Window::Window()
		:backendContext{nullptr},hglrc{nullptr},directContext{nullptr}
		,Hwnd{nullptr} ,BackgroundColor{ RRS::GetColor(255, 255, 255, 255) }	
	{
		App::Get()->Windows.push_back(this);
	}
	bool Window::Load() 
	{
		auto flag = createNativeWindow(); 
		if (!flag) return flag;
		initSurface();
		OnLoad();
		EmitEvent(EventType::Loaded);
		return true;
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
	void Window::AddElement(std::shared_ptr<Element> element)
	{
		addLayoutChild(element);
		Children.push_back(element);
	}
}