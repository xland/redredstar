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
		/// <summary>
		/// Constructor
		/// You can set the parameters of the window in the constructor of the subclass, 
		/// such as Height and Width
		/// </summary>
		WindowBase() = default;
		/// <summary>
		/// You need to release the objects of the subclass and the resources created by yourself.
		/// Don't worry about the resources created by WindowBase
		/// </summary>
		~WindowBase();
		/// <summary>
		/// Initialize window and drawing engine
		/// </summary>
		/// <returns>
		/// Returns false when there is an error
		/// </returns>
		bool Load();
		/// <summary>
		/// show the window
		/// </summary>
		void Show();
		/// <summary>
		/// close the window
		/// </summary>
		void Close();
		/// <summary>
		/// You can use this method to set the caption region of the window£¬
		/// when the window is frameless.
		/// </summary>
		/// <param name="x">Mouse position relative to the window x</param>
		/// <param name="y">Mouse position relative to the window Y</param>
		/// <returns>
		/// Returns true when the mouse is in the caption region
		/// </returns>
		virtual bool IsMouseInCaptionArea(int x, int y) { return false; };
		/// <summary>
		/// OnLoad method will be called after window and drawing engine are initialized
		/// </summary>
		virtual void OnLoad() = 0;
		/// <summary>
		/// OnClose method will be called before window closes
		/// </summary>
		/// <returns>
		/// return true to allow window close
		/// return false to block window close
		/// </returns>
		virtual bool OnClose();
		/// <summary>
		/// window's handle
		/// </summary>
		HWND hwnd;
		/// <summary>
		/// window position x
		/// ShowInCenterScreen must be set to false
		/// </summary>
		int X = CW_USEDEFAULT;
		/// <summary>
		/// window position y
		/// ShowInCenterScreen must be set to false
		/// </summary>
		int Y = CW_USEDEFAULT;
		/// <summary>
		/// window width
		/// </summary>
		int Width = 1000;
		/// <summary>
		/// window height
		/// </summary>
		int Height = 700;
		/// <summary>
		/// does window has frame
		/// </summary>
		bool Frame = true;
		/// <summary>
		/// is window shown in the center of the screen
		/// </summary>
		bool ShowInCenterScreen = true;
		/// <summary>
		/// window title
		/// </summary>
		std::wstring title = L"Window";
	protected:
	private:
		friend LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		LRESULT CALLBACK winProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		LRESULT hitTest(HWND hwnd, LPARAM lParam);
		void onPaint();
		DisplayParams* displayParams;
		WindowContext* windowContext;
		bool isContentInvalidated = true;

	};
}