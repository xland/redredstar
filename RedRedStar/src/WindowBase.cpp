#include "../include/RRS/WindowBase.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkSurface.h"
#include "../include/RRS/Element.h"
#include <GL/gl.h>

namespace RRS 
{
	WindowContext* pointer;
	WindowBase::WindowBase():backendContext{nullptr},hglrc{nullptr}
	{

	}
	bool WindowBase::Load() 
	{
		auto flag = createNativeWindow(); 
		if (!flag) return flag;
		initSurface();
		initLayout();
		OnLoad();
		return true;
	}
	void WindowBase::Close()
	{
		auto flag = OnClose();
		if (flag) {
			disposeLayout();
			DestroyWindow(hwnd);
		}
		OnClosed();
	}
	void WindowBase::paint() {
		calculateLayout();
		//SkSurface* surface = pointer->getBackbufferSurface(Width, Height).get();
		SkSurface* surface = getSurface(Width, Height);
		if (surface == nullptr) {
			return;
		}
		auto canvas = surface->getCanvas();
		canvas->clear(SK_ColorWHITE);
		for (auto element : Children)
		{
			element->Paint(canvas);
		}
		surface->flushAndSubmit();
		HDC dc = GetDC(hwnd);
		SwapBuffers(dc);
		ReleaseDC(hwnd, dc);
		delete surface;
		//todo destroy context
	}
	void WindowBase::Show() 
	{
		ShowWindow(hwnd, SW_SHOW);
	}
	void WindowBase::Hide() 
	{
		//todo
	}
}