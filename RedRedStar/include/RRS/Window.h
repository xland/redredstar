#pragma once
#include <Windows.h>
#include <memory>
#include <string>
#include <vector>
#include <future>
#include "EventListener.h"
#include "Element.h"
#include "Layout.h"
struct GrGLInterface;
class GrDirectContext;
class SkSurface;


namespace RRS {
	class DisplayParams;
	class WindowContext;
	class Window :public EventListener,public Layout
	{
	public:
		/// <summary>
		/// Constructor
		/// You can set the parameters of the window in the constructor of the subclass, 
		/// such as Height and Width
		/// </summary>
		Window();
		/// <summary>
		/// You need to release the objects of the subclass and the resources created by yourself.
		/// Don't worry about the resources created by WindowBase
		/// </summary>
		~Window();
		/// <summary>
		/// Initialize window and drawing engine
		/// </summary>
		/// <returns>
		/// Returns false when there is an error
		/// </returns>
		bool Load();
		/// <summary>
		/// Show the window
		/// </summary>
		void Show();
		/// <summary>
		/// Hide the window
		/// </summary>
		void Hide();
		/// <summary>
		/// Close the window
		/// </summary>
		void Close();
		void SetWidth(float width, bool isPercent = false) override {}; //todo assert
		void SetHeight(float height, bool isPercent = false) override {}; //todo assert
		void SetWidthReal(float widthReal) override {}; //todo assert
		void SetHeightReal(float heightReal) override {}; //todo assert
		void AddChild(std::shared_ptr<Element> child);
		/// <summary>
		/// window's handle
		/// </summary>
		HWND Hwnd = nullptr;
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
		/// is window shown in the center of the screen
		/// </summary>
		bool ShowInCenterScreen{ true };
		float Width{ 1000 };
		float Height{ 700 };
		int WidthMinimum{ 800 };
		int HeightMinimum{ 600 };
		int WidthMaximum{ 2000 };
		int HeightMaximum{ 1400 };
		bool CanMaximize{ true };
		bool CanResize{ true };
		bool HasFrame{ true };
		std::wstring WindowTitle{ L"Red Red Star Window" };
		Color BackgroundColor{ GetColor(255, 255, 255) };
	protected:
		/// <summary>
		/// OnLoad method will be called after window and drawing engine are initialized
		/// </summary>
		virtual void OnLoad(){};
		/// <summary>
		/// OnClose method will be called before window closes
		/// </summary>
		/// <returns>
		/// return true to allow window close
		/// return false to block window close
		/// </returns>
		virtual bool OnClose() { return true; };
		/// <summary>
		/// OnClosed method will be called after window closed
		/// </summary>
		virtual void OnClosed() {};
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
	private:
		friend LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		LRESULT CALLBACK winProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		LRESULT hitTest(HWND hwnd, LPARAM lParam);
		void initSurface();
		void paint();
		void paintLoopThread();
		std::shared_future<void> paintLoopThreadResult;
		bool createNativeWindow();
		SkSurface* getSurface();
		void disposeSurfaceResource();
		int stencilBits{0};
		HGLRC hglrc{ nullptr };
		const GrGLInterface* backendContext{ nullptr };
		GrDirectContext* directContext{ nullptr };
		
	};
}