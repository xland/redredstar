#pragma once
#include <Windows.h>
#include <memory>
#include <string>

namespace RRS {
	class DisplayParams;
	class WindowContext;

	class WindowBase
	{
	public:
		~WindowBase();
		bool Load();
		LRESULT CALLBACK winProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		void Show();
		virtual bool IsMouseInCaptionArea(int x, int y) = 0;
		virtual void OnLoad() = 0;
		HWND hwnd;
		int Width = 1000;
		int Height = 700;
		std::wstring title = L"Window";
	protected:
	private:
		/// <summary>
		/// 无边框窗口可拖拽区域
		/// </summary>
		/// <param name="hwnd"></param>
		/// <param name="lParam"></param>
		/// <returns></returns>
		LRESULT hitTest(HWND hwnd, LPARAM lParam);
		void onPaint();
		DisplayParams* displayParams;
		WindowContext* windowContext;
		bool isContentInvalidated = true;

	};
}