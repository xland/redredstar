#pragma once
#include <Windows.h>
#include "../../src/DisplayParams.h"
#include "../../src/WindowContext.h"
namespace RRS {
	class WindowBase
	{
	public:
		WindowBase(int width,int height);
		~WindowBase();
		LRESULT CALLBACK winProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		virtual bool IsMouseInCaptionArea(int x, int y) = 0;
		HWND hwnd;
		int Width;
		int Height;
	protected:
		DisplayParams requestedDisplayParams;
		std::unique_ptr<WindowContext> windowContext;
	private:
		/// <summary>
		/// 无边框窗口可拖拽区域
		/// </summary>
		/// <param name="hwnd"></param>
		/// <param name="lParam"></param>
		/// <returns></returns>
		LRESULT hitTest(HWND hwnd, LPARAM lParam);
		void onPaint();

	};
}