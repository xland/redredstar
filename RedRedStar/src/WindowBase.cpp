#include "../include/RRS/WindowBase.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkSurface.h"
#include "../include/RRS/Element.h"
#include <GL/gl.h>

namespace RRS 
{
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

	void WindowBase::Show() 
	{
		ShowWindow(hwnd, SW_SHOW);
	}
	void WindowBase::Hide() 
	{
		//todo
	}
}