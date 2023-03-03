#include "../include/RRS/Window.h"
#include "../include/RRS/App.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkSurface.h"
#include <Yoga.h>
#include <windowsx.h>
namespace RRS {
    LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) 
	{
        Window* win;
        if (msg == WM_CREATE) {
            win = (Window*)(((LPCREATESTRUCT)lParam)->lpCreateParams);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)win);
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }
        else {
            win = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
            if (!win) {
                return DefWindowProc(hwnd, msg, wParam, lParam);
            }
        }
        return win->winProc(hwnd, msg, wParam, lParam);
    }
    bool Window::createNativeWindow()
    {
		static std::wstring windowClassName = L"RRS_Window_Class";
		static WNDCLASSEX wcex;
		static bool wcexInit = false;
		if (!wcexInit) {
			wcex.cbSize = sizeof(WNDCLASSEX);
			wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
			wcex.lpfnWndProc = WindowProc;
			wcex.cbClsExtra = 0;
			wcex.cbWndExtra = 0;
			wcex.hInstance = App::Get()->HInstance;
			wcex.hIcon = LoadIcon(App::Get()->HInstance, (LPCTSTR)IDI_WINLOGO);
			wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
			wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
			wcex.lpszMenuName = nullptr;
			wcex.lpszClassName = windowClassName.c_str();
			wcex.hIconSm = LoadIcon(App::Get()->HInstance, (LPCTSTR)IDI_WINLOGO);
			if (!RegisterClassEx(&wcex)) {
				//todo log
				return false;
			}
			wcexInit = true;
		}
		if (ShowInCenterScreen) {
			RECT screenRect;
			SystemParametersInfo(SPI_GETWORKAREA, 0, &screenRect, 0);
			XWindow = (screenRect.right - width) / 2;
			YWindow = (screenRect.bottom - height) / 2;
		}
		Hwnd = CreateWindow(windowClassName.c_str(), title.c_str(), WS_OVERLAPPEDWINDOW, XWindow, YWindow, width, height,
			nullptr, nullptr, App::Get()->HInstance, nullptr);
		if (!Hwnd)
		{
			//todo log
			return false;
		}
		//RECT frame;
		//GetClientRect(hwnd, &frame);
		SetWindowLongPtr(Hwnd, GWLP_USERDATA, (LONG_PTR)this);
		RegisterTouchWindow(Hwnd, 0);
		return true;
    }
	LRESULT CALLBACK Window::winProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		PAINTSTRUCT ps;
		switch (msg) {
		case WM_PAINT:
			BeginPaint(hwnd, &ps);
			paint();
			EndPaint(hwnd, &ps);
		case WM_NCCALCSIZE: {
			//WINDOWPLACEMENT      wp;
			//LPNCCALCSIZE_PARAMS  szr;
			//wp.length = sizeof(WINDOWPLACEMENT);
			//GetWindowPlacement(hwnd, &wp);
			//szr = LPNCCALCSIZE_PARAMS(lParam);
			//if (szr) {
			//	if (szr && wp.showCmd == SW_SHOWMAXIMIZED) {
			//		RECT WorkAreaRect;
			//		SystemParametersInfo(SPI_GETWORKAREA, 0, &WorkAreaRect, 0);
			//		szr->rgrc[0] = WorkAreaRect;
			//	}
			//	//else if (wp.showCmd == SW_SHOWNORMAL) {
			//	//}
			//}
			//return 0;
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
		case WM_CLOSE: {
			Close();
			return 0;
		}
		case WM_LBUTTONUP: {
			for (auto element : Children)
			{
				element->EmitClickEvent();
			}
		}
		case WM_NCHITTEST: {
			return hitTest(hwnd, lParam);
		}
		case WM_GETMINMAXINFO: {
			MINMAXINFO* mminfo;
			mminfo = (PMINMAXINFO)lParam;
			mminfo->ptMinTrackSize.x = widthMinimum;
			mminfo->ptMinTrackSize.y = heightMinimum;
			mminfo->ptMaxPosition.x = 0;
			mminfo->ptMaxPosition.y = 0;
			return 0;
		}
		case WM_SIZE: {
			widthClient = LOWORD(lParam);
			heightClient = HIWORD(lParam);
			YGNodeStyleSetWidth(layout, widthClient);
			YGNodeStyleSetHeight(layout, heightClient);
			return 0;
		}
		case WM_MOUSEMOVE: {
			auto x = GET_X_LPARAM(lParam);
			auto y = GET_Y_LPARAM(lParam);
			mouseMove(x, y);
		}
		}
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	void Window::paint() {
		
		YGNodeCalculateLayout(layout, widthClient, heightClient, YGDirectionLTR);
		SkSurface* surface = getSurface();
		auto canvas = surface->getCanvas();
		canvas->clear(BackgroundColor);
		for (auto element : Children)
		{
			element->Paint(canvas);
		}
		surface->flushAndSubmit();
		HDC dc = GetDC(Hwnd);
		SwapBuffers(dc);
		ReleaseDC(Hwnd, dc);
		delete surface;
		//delete backendContext directContext
	}
	void Window::mouseMove(int x, int y)
	{
		for (auto element : Children)
		{
			element->SetIsMouseEnter(x, y);
		}
	}
	LRESULT Window::hitTest(HWND hwnd, LPARAM lParam) {
		POINT absoluteCursor = POINT{ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
		RECT winRect;
		GetWindowRect(hwnd, &winRect);
		if (absoluteCursor.x > winRect.left && absoluteCursor.y > winRect.top && absoluteCursor.x < winRect.right && absoluteCursor.y < winRect.bottom) {
			int borderWidth = 6;
			//Rml::Log::Message(Rml::Log::LT_ERROR, "%d < %d,%d < 50", absoluteCursor.x, winRect.right - 260, absoluteCursor.y);
			if (absoluteCursor.x < winRect.left + borderWidth && absoluteCursor.y < winRect.top + borderWidth) return HTTOPLEFT;
			else if (absoluteCursor.x < winRect.left + borderWidth && absoluteCursor.y > winRect.bottom - borderWidth) return HTBOTTOMLEFT;
			else if (absoluteCursor.x > winRect.right - borderWidth && absoluteCursor.y > winRect.bottom - borderWidth) return HTBOTTOMRIGHT;
			else if (absoluteCursor.x > winRect.right - borderWidth && absoluteCursor.y < winRect.top + borderWidth) return HTTOPRIGHT;
			else if (absoluteCursor.x < winRect.left + borderWidth) return HTLEFT;
			else if (absoluteCursor.x > winRect.right - borderWidth) return HTRIGHT;
			else if (absoluteCursor.y < winRect.top + borderWidth) return HTTOP;
			else if (absoluteCursor.y > winRect.bottom - borderWidth) return HTBOTTOM;
			else if (IsMouseInCaptionArea(absoluteCursor.x - winRect.left, absoluteCursor.y - winRect.top))
			{
				return HTCAPTION;
			}
			return HTCLIENT;
		}
		else
		{
			return HTNOWHERE;
		}
	}
}