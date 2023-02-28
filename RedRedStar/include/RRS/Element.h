#pragma once
#include "CommonType.h"
#include "Color.h"
#include "Layout.h"
#include "EventListener.h"
class SkCanvas;
namespace RRS {
	class Window;
	class Element:public Layout,public EventListener
	{
	public:
		Element();
		~Element();
		virtual void Paint(SkCanvas* canvas) = 0;
		/// <summary>
		/// show the element
		/// </summary>
		void Show();
		/// <summary>
		/// show the element
		/// </summary>
		void Hide();
		
		
		/// <summary>
		/// Get the element's owner window.
		/// </summary>
		/// <returns></returns>
		Window* OwnerWindow;
		/// <summary>
		/// 
		/// </summary>
		Element* ParentElement;		
		virtual void calculatePosition();
		virtual void SetIsMouseEnter(int x, int y);
		bool GetIsMouseEnter();
		
	private:
		friend Window;
		bool isMouseEnter = false;
	};
}