#pragma once
#include "CommonType.h"
#include "Color.h"
class SkCanvas;
namespace RRS {
	class WindowBase;
	class Layout;
	class Element
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
		
		void SetBackgroundColor(Color);
		/// <summary>
		/// Get the element's owner window.
		/// </summary>
		/// <returns></returns>
		WindowBase* GetOwnerWindow();
		Color BackgroundColor;
		/// <summary>
		/// 
		/// </summary>
		Element* ParentElement;
		Layout* Layout;
		
	private:

	};
}