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
		/// element position x
		/// ShowInCenterScreen must be set to false
		/// </summary>
		int X = 0;
		/// <summary>
		/// element position y
		/// ShowInCenterScreen must be set to false
		/// </summary>
		int Y = 0;
		/// <summary>
		/// element width
		/// </summary>
		float Width = 200;
		/// <summary>
		/// element height
		/// </summary>
		float Height = 200;
		Color BackgroundColor;
		/// <summary>
		/// The window pointer this element belong to
		/// </summary>
		WindowBase* OwnerWindow = nullptr;
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