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
		void Paint(SkCanvas* canvas);
		/// <summary>
		/// show the element
		/// </summary>
		void Show();
		/// <summary>
		/// show the element
		/// </summary>
		void Hide();
		/// <summary>
		/// Add an element to the window
		/// </summary>
		void AddElement(Element* element);
		
		void SetBackgroundColor(Color);
		/// <summary>
		/// Get the element's owner window.
		/// </summary>
		/// <returns></returns>
		WindowBase* GetOwnerWindow();
		/// <summary>
		/// element position x
		/// ShowInCenterScreen must be set to false
		/// </summary>
		int X = 0;
		/// <summary>
		/// element position y
		/// ShowInCenterScreen must be set to false
		/// </summary>
		int Y = 0;
		Color BackgroundColor;
		/// <summary>
		/// 
		/// </summary>
		Element* ParentElement;
		/// <summary>
		/// 
		/// </summary>
		std::vector<Element*> Children;
		Layout* Layout;
		
	private:

	};
}