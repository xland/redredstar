#include "../include/RRS/WindowBase.h"
#include "../include/RRS/App.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkSurface.h"
#include "DisplayParams.h"
#include "WindowContext.h"
#include <windowsx.h>
namespace RRS {
    LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        WindowBase* win;
        if (msg == WM_CREATE) {
            win = (WindowBase*)(((LPCREATESTRUCT)lParam)->lpCreateParams);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)win);
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }
        else {
            win = (WindowBase*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
            if (!win) {
                return DefWindowProc(hwnd, msg, wParam, lParam);
            }
        }
        return win->winProc(hwnd, msg, wParam, lParam);
    }
	bool WindowBase::Load() {
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
			X = (screenRect.right - Width) / 2; 
			Y = (screenRect.bottom - Height) / 2;
		}
		hwnd = CreateWindow(windowClassName.c_str(), title.c_str(), WS_OVERLAPPEDWINDOW, X, Y, Width, Height,
			nullptr, nullptr, App::Get()->HInstance, nullptr);
		if (!hwnd)
		{
			//todo log
			return false;
		}
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)this);
		RegisterTouchWindow(hwnd, 0);
		displayParams = new DisplayParams();
		windowContext = new WindowContext(hwnd, displayParams);
		OnLoad();
		return true;
	}
	void WindowBase::Close()
	{
		auto flag = OnClose();
		if (flag) {
			delete displayParams;
			delete windowContext;
			DestroyWindow(hwnd);
		}
		OnClosed();
	}

	LRESULT CALLBACK WindowBase::winProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		PAINTSTRUCT ps;
		switch (msg) {
		case WM_PAINT:
			BeginPaint(hwnd, &ps);
			onPaint();
			EndPaint(hwnd, &ps);
		case WM_NCCALCSIZE: {
			WINDOWPLACEMENT      wp;
			LPNCCALCSIZE_PARAMS  szr;
			wp.length = sizeof(WINDOWPLACEMENT);
			GetWindowPlacement(hwnd, &wp);
			szr = LPNCCALCSIZE_PARAMS(lParam);
			if (wp.showCmd == SW_SHOWMAXIMIZED) {
				RECT WorkAreaRect;
				SystemParametersInfo(SPI_GETWORKAREA, 0, &WorkAreaRect, 0);
				szr->rgrc[0] = WorkAreaRect;
			}
			else if (wp.showCmd == SW_SHOWNORMAL) {
			}
			return 0;
		}
		case WM_CLOSE: {
			Close();
			return 0;
		}
		case WM_NCHITTEST: {
			return hitTest(hwnd, lParam);
		}
		case WM_GETMINMAXINFO: {
			MINMAXINFO* mminfo;
			mminfo = (PMINMAXINFO)lParam;
			mminfo->ptMinTrackSize.x = 1024;
			mminfo->ptMinTrackSize.y = 768;
			mminfo->ptMaxPosition.x = 0;
			mminfo->ptMaxPosition.y = 0;
			return 0;
		}
		case WM_SIZE: {
			Width = LOWORD(lParam);
			Height = HIWORD(lParam);
			return 0;
		}
		}
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	LRESULT WindowBase::hitTest(HWND hwnd, LPARAM lParam) {
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
	void WindowBase::onPaint() {
		sk_sp<SkSurface> backbuffer = windowContext->getBackbufferSurface();
		if (backbuffer == nullptr) {
			return;
		}
		isContentInvalidated = false;
		SkSurface* surface = backbuffer.get();
		auto canvas = surface->getCanvas();
		canvas->clear(SK_ColorWHITE);
		SkPaint paint;
		paint.setColor(SkColorSetARGB(255, 220, 220, 220));
		paint.setStrokeJoin(SkPaint::Join::kRound_Join);
		SkRect rect = SkRect::MakeXYWH(20, 20, 180, 50);
		canvas->drawRoundRect(rect, 12, 32, paint);
		backbuffer->flushAndSubmit();
		windowContext->swapBuffers();
	}
	void WindowBase::Show() {
		ShowWindow(hwnd, SW_SHOW);
	}
}