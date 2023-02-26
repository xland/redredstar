#include "../include/RRS/App.h"
#include "../include/RRS/Window.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkSurface.h"
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
		
		return true;
	}
	void Window::Close()
	{
		auto flag = OnClose();
		if (flag) {
			App::Get()->RemoveWindow(this);
			disposeSurfaceResource();
			DestroyWindow(Hwnd);
		}
		OnClosed();
	}
	void Window::paint() {
		calculateLayout(Width,Height);
		SkSurface* surface = getSurface(Width, Height);
		if (surface == nullptr) {
			return;
		}
		auto canvas = surface->getCanvas();
		canvas->clear(BackgroundColor);
		for (auto element : Children)
		{
			canvas->save();
			element->Paint(canvas);
			canvas->restore();
		}
		surface->flushAndSubmit();
		HDC dc = GetDC(Hwnd);
		SwapBuffers(dc);
		ReleaseDC(Hwnd, dc);
		delete surface;
		//todo destroy context
	}
	void Window::Show() 
	{
		ShowWindow(Hwnd, SW_SHOW);
	}
	void Window::Hide() 
	{
		//todo
	}
	void Window::AddElement(Element* element)
	{
		addLayoutChild(element);
		Children.push_back(element);
	}
}