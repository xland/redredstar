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
	void Window::SetSize(float w, float h)
	{
		if (w < widthMinimum) {
			w = widthMinimum;
		}
		if (h < heightMinimum) {
			h = heightMinimum;
		}
		width = w;
		height = h;
	}
	float Window::GetWidth()
	{
		return width;
	}
	float Window::GetHeight()
	{
		return height;
	}
	void Window::SetWidth(float w)
	{
		if (w < widthMinimum) {
			w = widthMinimum;
		}
		width = w;
	}

	void Window::SetHeight(float h)
	{
		if (h < heightMinimum) {
			h = heightMinimum;
		}
		width = h;
	}

	int Window::GetWidthMinimum()
	{
		return widthMinimum;
	}
	int Window::GetHeightMinimum()
	{
		return heightMinimum;
	}
	void Window::SetWidthMinimum(int w)
	{
		//todo
		widthMinimum = w;
	}
	void Window::SetHeightMinimum(int h)
	{
		//todo
		heightMinimum = h;
	}
	void Window::SetSizeMinimum(int w, int h)
	{
		//todo
		widthMinimum = w;
		heightMinimum = h;
	}

	int Window::GetWidthMaximum()
	{
		return widthMaximum;
	}
	int Window::GetHeightMaximum()
	{
		return heightMaximum;
	}
	void Window::SetWidthMaximum(int w)
	{
		//todo
		widthMaximum = w;
	}
	void Window::SetHeightMaximum(int h)
	{
		//todo
		heightMaximum = h;
	}
	void Window::SetSizeMaximum(int w, int h)
	{
		//todo
		widthMaximum = w;
		heightMaximum = h;
	}


	bool Window::Load() 
	{
		auto flag = createNativeWindow(); 
		Layout::SetSize(width, height);
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
	void Window::AddChildElement(std::shared_ptr<Element> element)
	{
		//todo assert not loaded
		element->ownerWindow = this;
		addLayoutChild(element.get());
		Children.push_back(element);
	}
}